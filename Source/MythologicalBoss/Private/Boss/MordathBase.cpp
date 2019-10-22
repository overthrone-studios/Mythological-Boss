// Copyright Overthrone Studios 2019

#include "MordathBase.h"

#include "OverthroneGameState.h"

#include "BossAIController.h"
#include "MordathAnimInstance.h"

#include "Combat/MordathActionData.h"
#include "Misc/MordathStageData.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "FSM.h"

#include "ConstructorHelpers.h"
#include "OverthroneFunctionLibrary.h"

AMordathBase::AMordathBase() : AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Mordath/Animations/ABP_Mordath.ABP_Mordath_C'"));

	// Get the skeletal mesh to use
	SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/Characters/Mordath/SKM_Mordath.SKM_Mordath'")));

	// Configure our mesh
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -140.0f));
		GetMesh()->SetWorldScale3D(FVector(1.3f));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		if (AnimBP.Succeeded())
			GetMesh()->AnimClass = AnimBP.Class;
	}

	// Configure capsule component
	GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(140.0f, true);
	GetCapsuleComponent()->SetCapsuleRadius(90.0f, true);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	// Configure character settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossAIController::StaticClass();

	// Create a FSM
	FSM = CreateDefaultSubobject<UFSM>(FName("Boss FSM"));
	FSM->AddState(0, "Idle");
	FSM->AddState(1, "Follow");
	FSM->AddState(2, "Thinking");
	FSM->AddState(3, "Action");
	FSM->AddState(4, "Death");
	FSM->AddState(5, "Locked");

	FSM->OnEnterAnyState.AddDynamic(this, &AMordathBase::OnEnterAnyState);
	FSM->OnUpdateAnyState.AddDynamic(this, &AMordathBase::UpdateAnyState);
	FSM->OnExitAnyState.AddDynamic(this, &AMordathBase::OnExitAnyState);

	FSM->GetStateFromID(0)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterIdleState);
	FSM->GetStateFromID(0)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateIdleState);
	FSM->GetStateFromID(0)->OnExitState.AddDynamic(this, &AMordathBase::OnExitIdleState);

	FSM->GetStateFromID(1)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterFollowState);
	FSM->GetStateFromID(1)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateFollowState);
	FSM->GetStateFromID(1)->OnExitState.AddDynamic(this, &AMordathBase::OnExitFollowState);

	FSM->GetStateFromID(2)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterThinkState);
	FSM->GetStateFromID(2)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateThinkState);
	FSM->GetStateFromID(2)->OnExitState.AddDynamic(this, &AMordathBase::OnExitThinkState);

	FSM->GetStateFromID(3)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterActionState);
	FSM->GetStateFromID(3)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateActionState);
	FSM->GetStateFromID(3)->OnExitState.AddDynamic(this, &AMordathBase::OnExitActionState);
	
	FSM->GetStateFromID(4)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterDeathState);
	FSM->GetStateFromID(4)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateDeathState);
	FSM->GetStateFromID(4)->OnExitState.AddDynamic(this, &AMordathBase::OnExitDeathState);

	FSM->GetStateFromID(5)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterLockedState);
	FSM->GetStateFromID(5)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateLockedState);
	FSM->GetStateFromID(5)->OnExitState.AddDynamic(this, &AMordathBase::OnExitLockedState);

	FSM->InitFSM(0);

	// Create a range FSM
	RangeFSM = CreateDefaultSubobject<UFSM>(FName("Range FSM"));
	RangeFSM->AddState(0, "Close");
	RangeFSM->AddState(1, "Mid");
	RangeFSM->AddState(2, "Far");
	RangeFSM->AddState(3, "Super Close");

	RangeFSM->OnEnterAnyState.AddDynamic(this, &AMordathBase::OnEnterAnyRangeState);
	RangeFSM->OnUpdateAnyState.AddDynamic(this, &AMordathBase::UpdateAnyRangeState);
	RangeFSM->OnExitAnyState.AddDynamic(this, &AMordathBase::OnExitAnyRangeState);

	RangeFSM->GetStateFromID(0)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterCloseRange);
	RangeFSM->GetStateFromID(0)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateCloseRange);
	RangeFSM->GetStateFromID(0)->OnExitState.AddDynamic(this, &AMordathBase::OnExitCloseRange);

	RangeFSM->GetStateFromID(1)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterMidRange);
	RangeFSM->GetStateFromID(1)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateMidRange);
	RangeFSM->GetStateFromID(1)->OnExitState.AddDynamic(this, &AMordathBase::OnExitMidRange);

	RangeFSM->GetStateFromID(2)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterFarRange);
	RangeFSM->GetStateFromID(2)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateFarRange);
	RangeFSM->GetStateFromID(2)->OnExitState.AddDynamic(this, &AMordathBase::OnExitFarRange);

	RangeFSM->GetStateFromID(3)->OnEnterState.AddDynamic(this, &AMordathBase::OnEnterSuperCloseRange);
	RangeFSM->GetStateFromID(3)->OnUpdateState.AddDynamic(this, &AMordathBase::UpdateSuperCloseRange);
	RangeFSM->GetStateFromID(3)->OnExitState.AddDynamic(this, &AMordathBase::OnExitSuperCloseRange);

	RangeFSM->InitFSM(0);
}

void AMordathBase::BeginPlay()
{
	Super::BeginPlay();
	
	MordathAnimInstance = Cast<UMordathAnimInstance>(SKMComponent->GetAnimInstance());
	PlayerCharacter = UOverthroneFunctionLibrary::GetPlayerCharacter(this);
}

void AMordathBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((bIsDead || GameState->IsPlayerDead()) && !FSM->IsMachineRunning())
	{
		AnimInstance->MovementSpeed = 0.0f;
		AnimInstance->ForwardInput = 0.0f;
		AnimInstance->RightInput = 0.0f;
		MordathAnimInstance->CurrentStage = Stage_None;
		return;
	}
	
	DistanceToPlayer = GetDistanceToPlayer();
	DirectionToPlayer = GetDirectionToPlayer();

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->ForwardInput = ForwardInput;
	AnimInstance->RightInput = RightInput;
}

void AMordathBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
}

#pragma region Mordath Base Any States
#pragma region Main FSM
void AMordathBase::OnEnterAnyState(int32 ID, FName Name)
{
}

void AMordathBase::UpdateAnyState(int32 ID, FName Name, float Uptime, int32 Frames)
{
	if (GameState->IsPlayerDead() && HasFinishedAction())
	{
		FSM->Stop();
	}
}

void AMordathBase::OnExitAnyState(int32 ID, FName Name)
{
}
#pragma endregion

#pragma region Range FSM
void AMordathBase::OnEnterAnyRangeState(int32 ID, FName Name)
{
}

void AMordathBase::UpdateAnyRangeState(int32 ID, FName Name, float Uptime, int32 Frames)
{
}

void AMordathBase::OnExitAnyRangeState(int32 ID, FName Name)
{
}
#pragma endregion
#pragma endregion

#pragma region Mordath Base States
#pragma region Idle
void AMordathBase::OnEnterIdleState()
{
}

void AMordathBase::UpdateIdleState(float Uptime, int32 Frames)
{
	if (GameState->PlayerData.bIsDead)
		return;

	FacePlayer();

	StopMovement();

	FSM->PushState("Thinking");
}

void AMordathBase::OnExitIdleState()
{
}
#pragma endregion

#pragma region Follow
void AMordathBase::OnEnterFollowState()
{
	if (ChosenCombo->IsAtLastAction() && !IsWaitingForNextCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboDelayed();
		else
			ChooseCombo();
	}

	ChooseMovementDirection();
}

void AMordathBase::UpdateFollowState(float Uptime, int32 Frames)
{
	FacePlayer();

	// Decide a direction to move in
	if (!IsDelayingAction())
	{
		switch (CurrentActionData->RangeToExecute)
		{
		case BRM_SuperClose:
			if (IsCloseRange() || IsMidRange() || IsFarRange())
			{
				MoveForward();
			}
			else
			{
				StopMovement();
			}
		break;

		case BRM_Close:
			if (IsSuperCloseRange())
			{
				MoveForward(-1);
			}
			else if (IsMidRange() || IsFarRange())
			{
				MoveForward();
			}
			else
			{
				StopMovement();
			}
		break;

		case BRM_Mid:
			if (IsSuperCloseRange() || IsCloseRange())
			{
				MoveForward(-1);
			}
			else if (IsFarRange())
			{
				MoveForward();
			}
			else
			{
				StopMovement();
			}
		break;

		case BRM_Far:
			if (IsSuperCloseRange() || IsCloseRange() || IsMidRange())
			{
				MoveForward(-1);
			}
			else
			{
				StopMovement();
			}
		break;

		default:
			StopMovement();
		break;
		}
	}
	else
		FSM->PushState("Thinking");
}

void AMordathBase::OnExitFollowState()
{
}
#pragma endregion

#pragma region Think
void AMordathBase::OnEnterThinkState()
{
	ChooseMovementDirection();

	MordathAnimInstance->bIsThinking = true;
	MordathAnimInstance->bWantsSideStepDash = FMath::RandRange(0, 1);
}

void AMordathBase::UpdateThinkState(float Uptime, int32 Frames)
{
	if (IsFarRange())
	{
		FSM->PopState();
		FSM->PushState("Follow");
		return;
	}

	FacePlayer();

	EncirclePlayer();
}

void AMordathBase::OnExitThinkState()
{
	MordathAnimInstance->bIsThinking = false;
}
#pragma endregion

#pragma region Action
void AMordathBase::OnEnterActionState()
{
}

void AMordathBase::UpdateActionState(float Uptime, int32 Frames)
{
}

void AMordathBase::OnExitActionState()
{
}
#pragma endregion
#pragma endregion

#pragma region Mordath Base Range States
#pragma region Close Range
void AMordathBase::OnEnterCloseRange()
{
}

void AMordathBase::UpdateCloseRange(float Uptime, int32 Frames)
{
}

void AMordathBase::OnExitCloseRange()
{
}
#pragma endregion

#pragma region Mid Range
void AMordathBase::OnEnterMidRange()
{
}

void AMordathBase::UpdateMidRange(float Uptime, int32 Frames)
{
}

void AMordathBase::OnExitMidRange()
{
}
#pragma endregion

#pragma region Far Range
void AMordathBase::OnEnterFarRange()
{
}

void AMordathBase::UpdateFarRange(float Uptime, int32 Frames)
{
}

void AMordathBase::OnExitFarRange()
{
}
#pragma endregion

#pragma region Super Close Range
void AMordathBase::OnEnterSuperCloseRange()
{
}

void AMordathBase::UpdateSuperCloseRange(float Uptime, int32 Frames)
{
}

void AMordathBase::OnExitSuperCloseRange()
{
}
#pragma endregion
#pragma endregion

void AMordathBase::Die()
{
	Super::Die();

	MordathAnimInstance->LeaveAllStates();

	BossAIController->StopMovement();
	MovementComponent->SetMovementMode(MOVE_None);
	MovementComponent->DisableMovement();

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FSM->RemoveAllStates();
}

void AMordathBase::FacePlayer(const float RotationSpeed)
{
	if (RotationSpeed > 0.0f)
		SetActorRotation(FMath::Lerp(ControlRotation, FRotator(ControlRotation.Pitch, DirectionToPlayer.Rotation().Yaw, ControlRotation.Roll), RotationSpeed * World->DeltaTimeSeconds));
}

void AMordathBase::FacePlayer_Instant()
{
	SetActorRotation(FRotator(ControlRotation.Pitch, DirectionToPlayer.Rotation().Yaw, ControlRotation.Roll));
}

void AMordathBase::MoveForward(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * DirectionToPlayer);
	ForwardInput = Scale;
	RightInput = 0.0f;
}

void AMordathBase::MoveRight(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * GetActorRightVector());
	ForwardInput = 0.0f;
	RightInput = Scale;
}

void AMordathBase::ChooseMovementDirection()
{
	MovementDirection = FMath::RandRange(0, 1);
}

float AMordathBase::GetMovementSpeed() const
{
	return CurrentStageData->GetRunSpeed();
}

void AMordathBase::StopMovement()
{
	Super::StopMovement();

	BossAIController->StopMovement();
}

void AMordathBase::PlayActionMontage()
{
	PlayAnimMontage(CurrentActionMontage);
}

void AMordathBase::PlayActionMontage(UMordathActionData* ActionData)
{
	PlayAnimMontage(ActionData->ActionMontage);
}

void AMordathBase::StopActionMontage()
{
	if (!GameState->IsPlayerDead())
		StopAnimMontage();
}

void AMordathBase::ChooseCombo()
{
	static int8 ComboIndex = 0;

	if (CurrentStageData->ComboSettings.bChooseRandomCombo)
		ComboIndex = FMath::RandRange(0, CachedCombos.Num()-1);

	if (CachedCombos.Num() > 0)
	{
		// Is the combo data asset valid at 'Index'
		if (CachedCombos[ComboIndex])
		{
			ChosenCombo = CachedCombos[ComboIndex];

			ChosenCombo->Init();
			CurrentActionData = &ChosenCombo->GetCurrentActionData();
			CurrentActionMontage = CurrentActionData->Action->ActionMontage;
			
			StartExecutionExpiryTimer();

			CachedCombos.Remove(ChosenCombo);
		}
		else
		{
			#if !UE_BUILD_SHIPPING
			ULog::DebugMessage(WARNING, FString("Combo asset at index ") + FString::FromInt(ComboIndex) + FString(" is not valid"), true);
			#endif
		}
	}
	else
	{
		ComboIndex = 0;

		CachedCombos = CurrentStageData->ComboSettings.Combos;

		ChooseCombo();
	}
}

float AMordathBase::ChooseComboDelayed()
{
	if (CurrentStageData->ComboSettings.RandomDeviation == 0.0f)
	{
		TimerManager->SetTimer(TH_NextComboDelay, this, &AMordathBase::ChooseCombo, CurrentStageData->ComboSettings.ComboDelayTime);
		return CurrentStageData->ComboSettings.ComboDelayTime;
	}

	const float Min = CurrentStageData->ComboSettings.ComboDelayTime - CurrentStageData->ComboSettings.RandomDeviation;
	const float Max = CurrentStageData->ComboSettings.ComboDelayTime + CurrentStageData->ComboSettings.RandomDeviation;
	const float NewDelayTime = FMath::FRandRange(Min, Max);
				
	TimerManager->SetTimer(TH_NextComboDelay, this, &AMordathBase::ChooseCombo, NewDelayTime);

	return NewDelayTime;
}

bool AMordathBase::IsWaitingForNextCombo() const
{
	return TimerManager->IsTimerActive(TH_NextComboDelay);
}

void AMordathBase::ChooseAction()
{

}

void AMordathBase::ExecuteAction(UMordathActionData* ActionData)
{
	StopMovement();

	CurrentActionData->Action = ActionData;
}

bool AMordathBase::CanAttack() const
{
	return	CurrentActionData->RangeToExecute == BRM_AnyRange || 
			ChosenCombo->WantsToExecuteNonStop() &&
			!IsAttacking();
}

void AMordathBase::NextAction()
{
	if (ChosenCombo->IsDelayEnabled() && !IsDelayingAction())
	{
		const float Min = FMath::Clamp(ChosenCombo->GetActionDelayTime() - ChosenCombo->GetDeviation(), 0.0f, 100.0f);
		const float Max = FMath::Clamp(ChosenCombo->GetActionDelayTime() + ChosenCombo->GetDeviation(), 0.0f, 100.0f + ChosenCombo->GetDeviation());
		const float NewDelay = FMath::RandRange(Min, Max);

		if (NewDelay > 0.0f)
		{
			TimerManager->SetTimer(ChosenCombo->GetActionDelayTimer(), this, &AMordathBase::NextAction, NewDelay);
		}
		else
		{
			ChosenCombo->NextAction();
			StartExecutionExpiryTimer();
		}

		return;
	}

	ChosenCombo->NextAction();
	StartExecutionExpiryTimer();
}

void AMordathBase::StartExecutionExpiryTimer()
{
	TimerManager->SetTimer(CurrentActionData->TH_ExecutionExpiry, this, &AMordathBase::OnExecutionTimeExpired, CurrentActionData->ExecutionTime, false);
}

void AMordathBase::OnExecutionTimeExpired()
{
	CurrentActionData->bExecutionTimeExpired = true;
}

void AMordathBase::OnPlayerDeath()
{
	BossAIController->StopMovement();

	FSM->RemoveAllStatesExceptActive();
}

float AMordathBase::GetDistanceToPlayer() const
{
	return (GameState->PlayerData.Location - CurrentLocation).Size();
}

FVector AMordathBase::GetDirectionToPlayer() const
{
	return (GameState->PlayerData.Location - CurrentLocation).GetSafeNormal();
}

bool AMordathBase::IsSuperCloseRange() const
{
	return RangeFSM->GetActiveStateID() == 3;
}

bool AMordathBase::IsCloseRange() const
{
	return RangeFSM->GetActiveStateID() == 0;
}

bool AMordathBase::IsMidRange() const
{
	return RangeFSM->GetActiveStateID() == 1;
}

bool AMordathBase::IsFarRange() const
{
	return RangeFSM->GetActiveStateID() == 2;
}

float AMordathBase::GetActionDamage() const
{
	return ActionDamage;
}

float AMordathBase::GetAttackRadius() const
{
	return CurrentStageData->GetAttackRadius();
}

float AMordathBase::GetRecentDamage() const
{
	return CurrentStageData->GetRecentDamage();
}

bool AMordathBase::IsAttacking() const
{
	return IsShortAttacking() || IsLongAttacking() || IsSpecialAttacking();
}

bool AMordathBase::IsShortAttacking() const
{
	return CurrentActionType == ATM_ShortAttack_1 || CurrentActionType == ATM_ShortAttack_2 || CurrentActionType == ATM_ShortAttack_3;
}

bool AMordathBase::IsLongAttacking() const
{
	return CurrentActionType == ATM_LongAttack_1 || CurrentActionType == ATM_LongAttack_2 || CurrentActionType == ATM_LongAttack_3;
}

bool AMordathBase::IsSpecialAttacking() const
{
	return CurrentActionType == ATM_SpecialAttack_1 || CurrentActionType == ATM_SpecialAttack_2 || CurrentActionType == ATM_SpecialAttack_3;
}

bool AMordathBase::IsDelayingAction() const
{
	return TimerManager->IsTimerActive(ChosenCombo->GetActionDelayTimer());
}

bool AMordathBase::IsIdling() const
{
	return FSM->GetActiveStateID() == 0;
}

bool AMordathBase::IsFollowing() const
{
	return FSM->GetActiveStateID() == 1;
}

bool AMordathBase::IsThinking() const
{
	return FSM->GetActiveStateID() == 2;
}

bool AMordathBase::IsPerformingAction() const
{
	return FSM->GetActiveStateID() == 3;
}

bool AMordathBase::IsLocked() const
{
	return FSM->GetActiveStateID() == 5;
}

bool AMordathBase::HasFinishedAction() const
{
	return !AnimInstance->Montage_IsPlaying(CurrentActionMontage);
}

bool AMordathBase::HasFinishedAction(UAnimMontage* ActionMontage) const
{
	return !AnimInstance->Montage_IsPlaying(ActionMontage);
}

UForceFeedbackEffect* AMordathBase::GetCurrentForceFeedbackEffect() const
{
	return CurrentActionData->Action->ForceFeedbackEffect;
}

bool AMordathBase::WantsMoveRight() const
{
	return MovementDirection;
}

void AMordathBase::EncirclePlayer()
{
	if (WantsMoveRight())
	{
		MoveRight();
	}
	else
	{
		MoveRight(-1.0f);
	}
}
