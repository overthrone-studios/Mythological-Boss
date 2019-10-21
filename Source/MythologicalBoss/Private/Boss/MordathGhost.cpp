// Copyright Overthrone Studios 2019

#include "MordathGhost.h"

#include "Boss/MordathAnimInstance.h"
#include "Boss/BossAIController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Misc/MordathStageData.h"

#include "OverthroneGameState.h"

#include "FSM.h"
#include "Log.h"

AMordathGhost::AMordathGhost() : AMordathBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GhostMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/Art/Materials/VFX/MI_MordathGhostTrail.MI_MordathGhostTrail'")));

	// Configure our mesh
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
		GetMesh()->SetMaterial(i, GhostMaterial);

	// Create a FSM
	FSM = CreateDefaultSubobject<UFSM>(FName("Boss FSM"));
	FSM->AddState(1, "Follow");
	FSM->AddState(2, "Thinking");
	FSM->AddState(3, "Action");
	FSM->AddState(13, "Death");

	FSM->GetStateFromID(1)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterFollowState);
	FSM->GetStateFromID(1)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateFollowState);
	FSM->GetStateFromID(1)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitFollowState);

	FSM->GetStateFromID(2)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterThinkState);
	FSM->GetStateFromID(2)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateThinkState);
	FSM->GetStateFromID(2)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitThinkState);

	FSM->GetStateFromID(3)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterActionState);
	FSM->GetStateFromID(3)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateActionState);
	FSM->GetStateFromID(3)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitActionState);
	
	FSM->GetStateFromID(13)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterDeathState);
	FSM->GetStateFromID(13)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateDeathState);
	FSM->GetStateFromID(13)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitDeathState);

	FSM->InitFSM(1);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	Tags.Empty();
	Tags.Add("Mordath-Ghost");
}

void AMordathGhost::BeginPlay()
{
	Super::BeginPlay();

	CurrentStageData = StageData;
	MordathAnimInstance->CurrentStage = Stage_1;

	ChooseCombo();

	FSM->Start();
}

void AMordathGhost::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

float AMordathGhost::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!DamageCauser->IsA(AMordathBase::StaticClass()))
	{
		if (!bIsDead)
			FSM->PushState("Death");
	}

	return DamageAmount;
}

#pragma region Boss States
#pragma region Follow
void AMordathGhost::OnEnterFollowState()
{
	MovementComponent->SetMovementMode(MOVE_Walking);
	MovementComponent->MaxWalkSpeed = GetMovementSpeed();

	if (!ChosenCombo)
	{
#if !UE_BUILD_SHIPPING
		ULog::DebugMessage(ERROR, FString(GetName() + ": There are no combos in the list. A crash will occur!"), true);
#endif
		return;
	}

	if (ChosenCombo->IsAtLastAction())
	{
		ChooseCombo();
	}

	ChooseMovementDirection();
}

void AMordathGhost::UpdateFollowState(float Uptime, int32 Frames)
{
	FacePlayer();

	if (DistanceToPlayer < CurrentStageData->GetMidRangeRadius() && IsDelayingAttack())
	{
		FSM->PushState("Thinking");
		return;
	}

	// Move towards the player
	if (DistanceToPlayer < CurrentStageData->GetCloseRangeRadius())
	{
		if (!IsAttacking())
			ChooseAttack();

		CurrentMovementSpeed = 0.0f;
		ForwardInput = 0.0f;
		RightInput = 0.0f;
	}
	else
	{
		MoveForward();
	}
}

void AMordathGhost::OnExitFollowState()
{
}
#pragma endregion

#pragma region Think
void AMordathGhost::OnEnterThinkState()
{
	ChooseMovementDirection();

	ThinkTime = CurrentStageData->ThinkStateData.CalculateThinkTime();

	MordathAnimInstance->bIsThinking = true;
	MordathAnimInstance->bWantsSideStepDash = FMath::RandRange(0, 1);
}

void AMordathGhost::UpdateThinkState(float Uptime, int32 Frames)
{
	FacePlayer();

	if (AnimInstance->AnimTimeRemaining > 0.2f)
		EncirclePlayer();

	if (Uptime > ThinkTime)
	{
		FSM->PopState();
		FSM->PushState("Follow");
		return;
	}

	if (IsFarRange())
	{
		FSM->PopState();
		FSM->PushState("Follow");
	}
}

void AMordathGhost::OnExitThinkState()
{
	MordathAnimInstance->bIsThinking = false;
}
#pragma endregion 

#pragma region Action
void AMordathGhost::OnEnterActionState()
{
	PlayAttackMontage();
}

void AMordathGhost::UpdateActionState(float Uptime, int32 Frames)
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
		FSM->PopState();
}

void AMordathGhost::OnExitActionState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();

	NextAttack();
}
#pragma endregion 

#pragma region Death
void AMordathGhost::OnEnterDeathState()
{
	bIsDead = true;
	GameState->BossData.bIsDead = true;
	AnimInstance->bIsDead = true;

	OnDeath();
}

void AMordathGhost::UpdateDeathState(float Uptime, int32 Frames)
{
	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->Stop();
}

void AMordathGhost::OnExitDeathState()
{
	bIsDead = false;
	GameState->BossData.bIsDead = false;
	AnimInstance->bIsDead = false;
}
#pragma endregion
#pragma endregion 

void AMordathGhost::ChooseCombo()
{
	static int8 ComboIndex = 0;

	if (CurrentStageData->ComboSettings.bChooseRandomCombo)
		ComboIndex = FMath::RandRange(0, CachedCombos.Num() - 1);

	if (CachedCombos.Num() > 0)
	{
		// Is the combo data asset valid at 'Index'
		if (CachedCombos[ComboIndex])
		{
			ChosenCombo = CachedCombos[ComboIndex];

			ChosenCombo->Init();
			CurrentAttackData = &ChosenCombo->GetCurrentActionData();

			CachedCombos.Remove(ChosenCombo);
		}

		MovementComponent->MaxWalkSpeed = GetMovementSpeed();
	}
	else
	{
		ComboIndex = 0;

		CachedCombos = CurrentStageData->ComboSettings.Combos;

		ChooseCombo();
	}
}

void AMordathGhost::NextAttack()
{
	if (ChosenCombo->IsDelayEnabled() && !IsDelayingAttack())
	{
		const float Min = FMath::Clamp(ChosenCombo->GetActionDelayTime() - ChosenCombo->GetDeviation(), 0.0f, 100.0f);
		const float Max = FMath::Clamp(ChosenCombo->GetActionDelayTime() + ChosenCombo->GetDeviation(), 0.0f, 100.0f + ChosenCombo->GetDeviation());
		const float NewDelay = FMath::RandRange(Min, Max);

		if (NewDelay > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(ChosenCombo->GetActionDelayTimer(), this, &AMordathGhost::NextAttack, NewDelay);
			MovementComponent->MaxWalkSpeed = MovementComponent->MaxWalkSpeed / 2.0f;
		}
		else
		{
			ChosenCombo->NextAction();
		}

		return;
	}

	ChosenCombo->NextAction();
}

void AMordathGhost::PlayAttackMontage()
{
	PlayAnimMontage(CurrentAttackData->Action->ActionMontage, 1.0f, FName("Anticipation"));
}

void AMordathGhost::StopAttackMontage()
{
	if (!HasFinishedAttack() && !GameState->IsPlayerDead())
		StopAnimMontage(CurrentAttackData->Action->ActionMontage);

	CurrentMontageSection = "None";
}

void AMordathGhost::ChooseAttack()
{
	if (IsAttacking())
		return;

	CurrentAttackData = &ChosenCombo->GetCurrentActionData();

	// Choose the current attack from the attack data
	switch (CurrentAttackData->Action->ActionType)
	{
	case ATM_ShortAttack_1:
		FSM->PushState("Light Attack 1");
		break;

	case ATM_ShortAttack_2:
		FSM->PushState("Light Attack 2");
		break;

	case ATM_ShortAttack_3:
		FSM->PushState("Light Attack 3");
		break;

	case ATM_LongAttack_1:
		FSM->PushState("Heavy Attack 1");
		break;

	case ATM_LongAttack_2:
		FSM->PushState("Heavy Attack 2");
		break;

	case ATM_LongAttack_3:
		FSM->PushState("Heavy Attack 3");
		break;

	default:
		break;
	}
}

bool AMordathGhost::IsDelayingAttack() const
{
	return TimerManager->IsTimerActive(ChosenCombo->GetActionDelayTimer());
}

bool AMordathGhost::IsFarRange() const
{
	return DistanceToPlayer > CurrentStageData->GetMidRangeRadius();
}

bool AMordathGhost::HasFinishedAttack() const
{
	return !AnimInstance->Montage_IsPlaying(CurrentAttackData->Action->ActionMontage);
}

bool AMordathGhost::IsShortAttacking() const
{
	return FSM->GetActiveStateID() == 3 && 
			CurrentActionType == ATM_ShortAttack_1 || 
			CurrentActionType == ATM_ShortAttack_2 || 
			CurrentActionType == ATM_ShortAttack_3;
}

bool AMordathGhost::IsLongAttacking() const
{
	return FSM->GetActiveStateID() == 3 && 
			CurrentActionType == ATM_LongAttack_1 || 
			CurrentActionType == ATM_LongAttack_2 || 
			CurrentActionType == ATM_LongAttack_3;
}

bool AMordathGhost::IsSpecialAttacking() const
{
	return FSM->GetActiveStateID() == 3 && 
			CurrentActionType == ATM_SpecialAttack_1 || 
			CurrentActionType == ATM_SpecialAttack_2 || 
			CurrentActionType == ATM_SpecialAttack_3;
}

float AMordathGhost::GetMovementSpeed() const
{
	if (DistanceToPlayer < CurrentStageData->GetCloseRangeRadius())
		return CurrentStageData->GetWalkSpeed();

	if (DistanceToPlayer > CurrentStageData->GetCloseRangeRadius())
		return CurrentStageData->GetRunSpeed();

	if (DistanceToPlayer > CurrentStageData->GetMidRangeRadius())
		return CurrentStageData->GetRunSpeed();

	return CurrentStageData->GetRunSpeed();
}

void AMordathGhost::PauseAnimsWithTimer()
{
	if (CurrentStageData->IsHitStopEnabled())
	{
		PauseAnims();
		TimerManager->SetTimer(HitStopTimerHandle, this, &AMordathGhost::UnPauseAnims, CurrentStageData->GetHitStopTime());
	}
}
