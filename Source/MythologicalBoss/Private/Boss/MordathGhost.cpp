// Copyright Overthrone Studios 2019

#include "MordathGhost.h"

#include "Boss/MordathAnimInstance.h"
#include "Boss/BossAIController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Misc/MordathStageData.h"

#include "OverthroneGameState.h"

#include "ConstructorHelpers.h"

#include "FSM.h"
#include "Log.h"

AMordathGhost::AMordathGhost() : AMordathBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Mordath/Animations/ABP_Mordath_Ghost.ABP_Mordath_Ghost_C'"));

	GhostMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/Art/Materials/VFX/MI_MordathGhostTrail.MI_MordathGhostTrail'")));

	// Configure our mesh
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
		GetMesh()->SetMaterial(i, GhostMaterial);

	if (AnimBP.Succeeded())
		GetMesh()->AnimClass = AnimBP.Class;

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
	Super::OnEnterFollowState();
}

void AMordathGhost::UpdateFollowState(float Uptime, int32 Frames)
{
	Super::UpdateFollowState(Uptime, Frames);

	if (IsWaitingForNextCombo() && IsCloseRange())
	{
		FSM->PushState("Thinking");
		return;
	}

	if (ChosenCombo->IsAtLastAction() && !IsWaitingForNextCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboDelayed();
		else
			ChooseCombo();

		return;
	}

	if (CanAttack())
	{
		// Decide an action to choose from
		if (!IsWaitingForNextCombo() && !IsDelayingAction())
		{
			ChooseAction();
		}
	}
}

void AMordathGhost::OnExitFollowState()
{
}
#pragma endregion

#pragma region Think
void AMordathGhost::OnEnterThinkState()
{
	Super::OnEnterThinkState();

	ThinkTime = CurrentStageData->ThinkStateData.CalculateThinkTime();
}

void AMordathGhost::UpdateThinkState(float Uptime, int32 Frames)
{
	Super::UpdateThinkState(Uptime, Frames);

	if (!IsWaitingForNextCombo() && Uptime > ThinkTime)
	{
		FSM->PopState();
		FSM->PushState("Follow");
	}
}

void AMordathGhost::OnExitThinkState()
{
	Super::OnExitThinkState();
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

void AMordathGhost::StopActionMontage()
{
	Super::StopActionMontage();

	CurrentMontageSection = "None";
}

void AMordathGhost::ChooseAction()
{
	if (IsAttacking())
		return;

	CurrentActionData = &ChosenCombo->GetCurrentActionData();

	// Update data
	CurrentActionType = CurrentActionData->Action->ActionType;
	CurrentCounterType = CurrentActionData->Action->CounterType;
	ActionDamage = CurrentActionData->Action->ActionDamage;

	ExecuteAction(CurrentActionData->Action);
}

void AMordathGhost::ExecuteAction(UMordathActionData* ActionData)
{
	Super::ExecuteAction(ActionData);

	FSM->PushState("Action");
}

void AMordathGhost::PauseAnimsWithTimer()
{
	if (CurrentStageData->IsHitStopEnabled())
	{
		PauseAnims();
		TimerManager->SetTimer(HitStopTimerHandle, this, &AMordathGhost::UnPauseAnims, CurrentStageData->GetHitStopTime());
	}
}
