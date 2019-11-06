// Copyright Overthrone Studios 2019

#include "MordathTutorial.h"

#include "Boss/MordathAnimInstance.h"
#include "Boss/BossAIController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Misc/MordathStageData.h"

#include "OverthroneGameState.h"

#include "ConstructorHelpers.h"

#include "FSM.h"

AMordathTutorial::AMordathTutorial()
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

	GetCapsuleComponent()->SetCollisionProfileName(FName("Mordath-Ghost"));

	FSM->InitFSM(1);

	Tags.Empty();
	Tags.Add("Mordath-Tutorial");
}

void AMordathTutorial::ResetSelf()
{
	CurrentActionIndex = FMath::Clamp(CurrentActionIndex + 1, 0, Actions.Num() - 1);

	bCurrentTutorialCompleted = false;
	bCanBeDamaged = false;
	bIsDead = false;
	AnimInstance->bIsDead = false;
	FSM->RemoveAllStates();
}

void AMordathTutorial::NotifyTutorialCompleted()
{
	ULog::Info("Completed", true);

	bCurrentTutorialCompleted = true;
}

float AMordathTutorial::GetMovementSpeed() const
{
	switch (RangeFSM->GetActiveStateID())
	{
	case 0 /*Close*/:
		return 500.0f;

	case 1 /*Mid*/:
		return 1000.0f;

	case 2 /*Far*/:
		return 1000.0f;

	default:
		return 500.0f;
	}
}

void AMordathTutorial::LockSelf()
{
	FSM->PushState("Locked");
}

void AMordathTutorial::UnlockSelf()
{
	FSM->PopState("Locked");
}

void AMordathTutorial::BeginPlay()
{
	Super::BeginPlay();

	MordathAnimInstance->CurrentStage = Stage_1;
	MordathAnimInstance->bIsThinking = false;

	CurrentActionIndex = 0;

	bCanBeDamaged = false;

	FacePlayer_Instant();

	FSM->Start();
	RangeFSM->Start();
}

float AMordathTutorial::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!DamageCauser->IsA(AMordathBase::StaticClass()))
	{
		if (!bIsDead)
			FSM->PushState("Death");
	}

	return DamageAmount;
}

bool AMordathTutorial::CanAttack() const
{
	return !IsAttacking() && IsCloseRange() && !IsLocked() && !bIsDead;
}

void AMordathTutorial::OnEnterFollowState()
{
	if (Actions[CurrentActionIndex].bKillOnActionCompleted && bCurrentTutorialCompleted)
	{
		if (!bIsDead)
			FSM->PushState("Death");
	}
}

void AMordathTutorial::UpdateFollowState(const float Uptime, const int32 Frames)
{
	if (bIsDead)
		return;

	FacePlayer();

	if (IsCloseRange())
		MoveForward();
	else if (IsSuperCloseRange())
		MoveForward(-1);
	else
		MoveForward();

	if (CanAttack())
	{
		ChooseAction();
	}
}

void AMordathTutorial::OnExitFollowState()
{
}

void AMordathTutorial::OnEnterActionState()
{		
	PlayActionMontage();

	StartActionLocation = CurrentLocation;
}

void AMordathTutorial::UpdateActionState(const float Uptime, const int32 Frames)
{
	if (bIsDead)
		return;

	StopMovement();

	if (!bIsDead)
		FacePlayerBasedOnActionData(Actions[CurrentActionIndex].ActionData);

	if (AnimInstance->Montage_GetPosition(CurrentActionMontage) >= Actions[CurrentActionIndex].PauseAtTime && !bStopAtTimeEventTriggered)
	{
		bCanBeDamaged = true;

		OnStopAtTimeReached.Broadcast();
		bStopAtTimeEventTriggered = true;
	}

	// If action has finished, go back to previous state
	if (HasFinishedAction() && !Actions[CurrentActionIndex].bKillOnActionCompleted && !bCurrentTutorialCompleted)
		PlayActionMontage();
	else if (HasFinishedAction())
		FSM->PopState();
}

void AMordathTutorial::OnExitActionState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopActionMontage();
}

void AMordathTutorial::OnEnterDeathState()
{
	AnimInstance->StopAllMontages(0.4f);
	
	AnimInstance->bIsDead = true;
	bIsDead = true;
	bCanBeDamaged = false;

	bStopAtTimeEventTriggered = false;

	PauseAnims();
	
	OnDeath();

	OnDead.Broadcast();
}

void AMordathTutorial::UpdateDeathState(float Uptime, int32 Frames)
{
	StopMovement();

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AMordathTutorial::OnExitDeathState()
{
	bIsDead = false;
	AnimInstance->bIsDead = false;

	MovementComponent->SetMovementMode(MOVE_Walking);

	FSM->Start();
}

void AMordathTutorial::UpdateLockedState(const float Uptime, const int32 Frames)
{
	Super::UpdateLockedState(Uptime, Frames);

	SetActorRotation(FRotator(ControlRotation.Pitch, GetDirectionToPlayer().Rotation().Yaw, ControlRotation.Roll));
}

#pragma region Mordath Tutorial Range States
#pragma region Close Range
void AMordathTutorial::OnEnterCloseRange()
{
}

void AMordathTutorial::UpdateCloseRange(float Uptime, int32 Frames)
{
	if (DistanceToPlayer < 300.0f)
		RangeFSM->PushState("Super Close");

	if (DistanceToPlayer > 600.0f)
		RangeFSM->PushState("Mid");
}

void AMordathTutorial::OnExitCloseRange()
{
}
#pragma endregion

#pragma region Mid Range
void AMordathTutorial::OnEnterMidRange()
{
}

void AMordathTutorial::UpdateMidRange(float Uptime, int32 Frames)
{
	if (DistanceToPlayer < 600.0f)
		RangeFSM->PushState("Close");

	if (DistanceToPlayer > 1200.0f)
		RangeFSM->PushState("Far");
}

void AMordathTutorial::OnExitMidRange()
{
}
#pragma endregion

#pragma region Far Range
void AMordathTutorial::OnEnterFarRange()
{
}

void AMordathTutorial::UpdateFarRange(float Uptime, int32 Frames)
{
	if (DistanceToPlayer < 1200.0f)
	{
		RangeFSM->PushState("Mid");
	}
}

void AMordathTutorial::OnExitFarRange()
{
}
#pragma endregion

#pragma region Super Close Range
void AMordathTutorial::OnEnterSuperCloseRange()
{
}

void AMordathTutorial::UpdateSuperCloseRange(float Uptime, int32 Frames)
{
	if (DistanceToPlayer > 300.0f)
	{
		RangeFSM->PushState("Close");
	}
}

void AMordathTutorial::OnExitSuperCloseRange()
{
}
#pragma endregion
#pragma endregion


void AMordathTutorial::StopActionMontage()
{
	Super::StopActionMontage();

	CurrentMontageSection = "None";
}

void AMordathTutorial::ChooseAction()
{
	if (IsAttacking() || bIsDead)
		return;
	
	CurrentActionMontage = Actions[CurrentActionIndex].ActionData->ActionMontage;

	// Update data
	CurrentActionType = Actions[CurrentActionIndex].ActionData->ActionType;
	CurrentCounterType = Actions[CurrentActionIndex].ActionData->CounterType;
	ActionDamage = 0;

	ExecuteAction(Actions[CurrentActionIndex].ActionData);
}

void AMordathTutorial::ExecuteAction(UMordathActionData* ActionData)
{
	StopMovement();

	FSM->PushState("Action");
}
