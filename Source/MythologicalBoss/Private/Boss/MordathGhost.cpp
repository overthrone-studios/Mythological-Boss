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
	RangeFSM->Start();
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
	Super::OnEnterActionState();
}

void AMordathGhost::UpdateActionState(float Uptime, int32 Frames)
{
	Super::UpdateActionState(Uptime, Frames);
}

void AMordathGhost::OnExitActionState()
{
	if (CurrentActionData->bExecutionTimeExpired)
	{
		StopActionMontage();
		NextAction();

		return;
	}

	Super::OnExitActionState();
}
#pragma endregion 

#pragma region Death
void AMordathGhost::OnEnterDeathState()
{
	Super::OnEnterDeathState();
	
	OnDeath();
}

void AMordathGhost::UpdateDeathState(float Uptime, int32 Frames)
{
	Super::UpdateDeathState(Uptime, Frames);
}

void AMordathGhost::OnExitDeathState()
{
	Super::OnExitDeathState();
}
#pragma endregion
#pragma endregion 

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

bool AMordathGhost::HasFinishedAttack() const
{
	return !AnimInstance->Montage_IsPlaying(CurrentAttackData->Action->ActionMontage);
}

void AMordathGhost::PauseAnimsWithTimer()
{
	if (CurrentStageData->IsHitStopEnabled())
	{
		PauseAnims();
		TimerManager->SetTimer(HitStopTimerHandle, this, &AMordathGhost::UnPauseAnims, CurrentStageData->GetHitStopTime());
	}
}
