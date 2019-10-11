// Copyright Overthrone Studios 2019

#include "MordathGhost.h"

#include "Ylva.h"

#include "Boss/MordathAnimInstance.h"
#include "Boss/BossAIController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Misc/MordathStageData.h"

#include "OverthroneFunctionLibrary.h"
#include "OverthroneGameState.h"

#include "ConstructorHelpers.h"
#include "FSM.h"
#include "Log.h"


AMordathGhost::AMordathGhost() : AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Mordath/Animations/ABP_Mordath.ABP_Mordath_C'"));

	// Get the skeletal mesh to use
	SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/Characters/Mordath/SKM_Mordath.SKM_Mordath'")));
	GhostMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/Art/Materials/VFX/MI_MordathGhostTrail.MI_MordathGhostTrail'")));

	// Configure our mesh
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);

		for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
			GetMesh()->SetMaterial(i, GhostMaterial);

		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -190.0f));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		if (AnimBP.Succeeded())
			GetMesh()->AnimClass = AnimBP.Class;
	}

	// Create a FSM
	FSM = CreateDefaultSubobject<UFSM>(FName("Boss FSM"));
	FSM->AddState(1, "Follow");
	FSM->AddState(2, "Thinking");
	FSM->AddState(3, "Light Attack 1");
	FSM->AddState(4, "Light Attack 2");
	FSM->AddState(5, "Light Attack 3");
	FSM->AddState(6, "Heavy Attack 1");
	FSM->AddState(7, "Heavy Attack 2");
	FSM->AddState(8, "Heavy Attack 3");
	FSM->AddState(13, "Death");

	FSM->GetStateFromID(1)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterFollowState);
	FSM->GetStateFromID(1)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateFollowState);
	FSM->GetStateFromID(1)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitFollowState);

	FSM->GetStateFromID(2)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterThinkState);
	FSM->GetStateFromID(2)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateThinkState);
	FSM->GetStateFromID(2)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitThinkState);

	FSM->GetStateFromID(3)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterShortAttack1State);
	FSM->GetStateFromID(3)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateShortAttack1State);
	FSM->GetStateFromID(3)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitShortAttack1State);

	FSM->GetStateFromID(4)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterShortAttack2State);
	FSM->GetStateFromID(4)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateShortAttack2State);
	FSM->GetStateFromID(4)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitShortAttack2State);

	FSM->GetStateFromID(5)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterShortAttack3State);
	FSM->GetStateFromID(5)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateShortAttack3State);
	FSM->GetStateFromID(5)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitShortAttack3State);

	FSM->GetStateFromID(6)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterLongAttack1State);
	FSM->GetStateFromID(6)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateLongAttack1State);
	FSM->GetStateFromID(6)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitLongAttack1State);
	
	FSM->GetStateFromID(7)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterLongAttack2State);
	FSM->GetStateFromID(7)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateLongAttack2State);
	FSM->GetStateFromID(7)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitLongAttack2State);
	
	FSM->GetStateFromID(8)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterLongAttack3State);
	FSM->GetStateFromID(8)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateLongAttack3State);
	FSM->GetStateFromID(8)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitLongAttack3State);
	
	FSM->GetStateFromID(13)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterDeathState);
	FSM->GetStateFromID(13)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateDeathState);
	FSM->GetStateFromID(13)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitDeathState);

	FSM->InitFSM(1);

	// Configure capsule component
	GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(140.0f, true);
	GetCapsuleComponent()->SetCapsuleRadius(90.0f, true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	// Configure character settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossAIController::StaticClass();
}

void AMordathGhost::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UOverthroneFunctionLibrary::GetPlayerCharacter(this);
	MordathAnimInstance = Cast<UMordathAnimInstance>(SKMComponent->GetAnimInstance());

	DistanceToPlayer = GetDistanceToPlayer();

	CurrentStageData = StageOneData;
	MordathAnimInstance->CurrentStage = Stage_1;

	ChooseCombo();

	FSM->Start();
}

void AMordathGhost::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsDead)
	{
		AnimInstance->MovementSpeed = 0.0f;
		AnimInstance->ForwardInput = ForwardInput;
		AnimInstance->RightInput = RightInput;
		return;
	}

	DistanceToPlayer = GetDistanceToPlayer();
	DirectionToPlayer = GetDirectionToPlayer();
	CurrentMovementSpeed = GetMovementSpeed();

	MovementComponent->MaxWalkSpeed = CurrentMovementSpeed;

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->ForwardInput = ForwardInput;
	AnimInstance->RightInput = RightInput;
}

void AMordathGhost::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
}

float AMordathGhost::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser->IsA(AYlva::StaticClass()))
	{
		if (!bIsDead)
			FSM->PushState("Death");
	}

	return DamageAmount;
}

void AMordathGhost::FacePlayer()
{
	SetActorRotation(FRotator(GetControlRotation().Pitch, DirectionToPlayer.Rotation().Yaw, GetControlRotation().Roll));
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

#pragma region Light Attack 1
void AMordathGhost::OnEnterShortAttack1State()
{
	PlayAttackMontage();
}

void AMordathGhost::UpdateShortAttack1State(float Uptime, int32 Frames)
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordathGhost::OnExitShortAttack1State()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion

#pragma region Light Attack 2
void AMordathGhost::OnEnterShortAttack2State()
{
	PlayAttackMontage();
}

void AMordathGhost::UpdateShortAttack2State(float Uptime, int32 Frames)
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordathGhost::OnExitShortAttack2State()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion

#pragma region Light Attack 3
void AMordathGhost::OnEnterShortAttack3State()
{
	PlayAttackMontage();
}

void AMordathGhost::UpdateShortAttack3State(float Uptime, int32 Frames)
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordathGhost::OnExitShortAttack3State()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion 

#pragma region Heavy Attack 1
void AMordathGhost::OnEnterLongAttack1State()
{
	GameState->BossData.CurrentActionType = ATM_LongAttack_1;

	CurrentLongAttackMontage = CurrentStageData->GetRandomFarRangeAction()->ActionMontage;
	PlayAnimMontage(CurrentLongAttackMontage);
}

void AMordathGhost::UpdateLongAttack1State(float Uptime, int32 Frames)
{
	CurrentMontageSection = AnimInstance->Montage_GetCurrentSection(CurrentLongAttackMontage);

	FacePlayer();

	// If attack animation has finished, go back to previous state
	if (!AnimInstance->Montage_IsPlaying(CurrentLongAttackMontage))
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordathGhost::OnExitLongAttack1State()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAnimMontage();
}
#pragma endregion

#pragma region Heavy Attack 2
void AMordathGhost::OnEnterLongAttack2State()
{
	PlayAttackMontage();
}

void AMordathGhost::UpdateLongAttack2State(float Uptime, int32 Frames)
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordathGhost::OnExitLongAttack2State()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion

#pragma region Heavy Attack 3
void AMordathGhost::OnEnterLongAttack3State()
{
	PlayAttackMontage();
}

void AMordathGhost::UpdateLongAttack3State(float Uptime, int32 Frames)
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordathGhost::OnExitLongAttack3State()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
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

float AMordathGhost::GetDistanceToPlayer() const
{
	return FVector::Dist(CurrentLocation, GameState->PlayerData.Location);
}

FVector AMordathGhost::GetDirectionToPlayer() const
{
	FVector Direction = GameState->PlayerData.Location - CurrentLocation;
	Direction.Normalize();

	return Direction;
}

void AMordathGhost::ChooseMovementDirection()
{
	MoveDirection = FMath::RandRange(0, 1);
}

void AMordathGhost::ChooseCombo()
{
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

bool AMordathGhost::IsDelayingAttack() const
{
	return TimerManager->IsTimerActive(ChosenCombo->GetActionDelayTimer());
}

void AMordathGhost::MoveForward(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * DirectionToPlayer);
	ForwardInput = Scale;
	RightInput = 0.0f;
}

void AMordathGhost::MoveRight(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * GetActorRightVector());
	ForwardInput = 0.0f;
	RightInput = Scale;
}

float AMordathGhost::GetShortAttackDamage() const
{
	return CurrentAttackData->Action->ActionDamage;
}

float AMordathGhost::GetLongAttackDamage() const
{
	return CurrentAttackData->Action->ActionDamage;
}

float AMordathGhost::GetSpecialAttackDamage() const
{
	return CurrentAttackData->Action->ActionDamage;
}

float AMordathGhost::GetAttackRadius() const
{
	return CurrentStageData->GetAttackRadius();
}

bool AMordathGhost::IsFarRange() const
{
	return DistanceToPlayer > CurrentStageData->GetMidRangeRadius();
}

bool AMordathGhost::HasFinishedAttack() const
{
	return !AnimInstance->Montage_IsPlaying(CurrentAttackData->Action->ActionMontage);
}

bool AMordathGhost::IsAttacking() const
{
	return IsShortAttacking() || IsLongAttacking() || IsSpecialAttacking();
}

bool AMordathGhost::IsShortAttacking() const
{
	return FSM->GetActiveStateID() == 3 || FSM->GetActiveStateID() == 4 || FSM->GetActiveStateID() == 5;
}

bool AMordathGhost::IsLongAttacking() const
{
	return FSM->GetActiveStateID() == 6 || FSM->GetActiveStateID() == 7 || FSM->GetActiveStateID() == 8;
}

bool AMordathGhost::IsSpecialAttacking() const
{
	return FSM->GetActiveStateID() == 9 || FSM->GetActiveStateID() == 10 || FSM->GetActiveStateID() == 11;
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

void AMordathGhost::EncirclePlayer()
{
	MovementComponent->MaxWalkSpeed = CurrentStageData->GetWalkSpeed();

	if (PlayerCharacter->GetInputAxisValue("MoveRight") > 0.0f && PlayerCharacter->HasMovedRightBy(300.0f))
	{
		MoveRight();
	}
	else if (PlayerCharacter->GetInputAxisValue("MoveRight") < 0.0f && PlayerCharacter->HasMovedLeftBy(300.0f))
	{
		MoveRight(-1.0f);
	}
	else
	{
		if (MoveDirection == 1)
		{
			MoveRight();
		}
		else
		{
			MoveRight(-1.0f);
		}
	}
}
