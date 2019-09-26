// Copyright Overthrone Studios 2019

#include "MordathGhost.h"

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

	FSM->GetState(1)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterFollowState);
	FSM->GetState(1)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateFollowState);
	FSM->GetState(1)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitFollowState);

	FSM->GetState(2)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterThinkState);
	FSM->GetState(2)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateThinkState);
	FSM->GetState(2)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitThinkState);

	FSM->GetState(3)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterShortAttack1State);
	FSM->GetState(3)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateShortAttack1State);
	FSM->GetState(3)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitShortAttack1State);

	FSM->GetState(4)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterShortAttack2State);
	FSM->GetState(4)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateShortAttack2State);
	FSM->GetState(4)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitShortAttack2State);

	FSM->GetState(5)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterShortAttack3State);
	FSM->GetState(5)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateShortAttack3State);
	FSM->GetState(5)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitShortAttack3State);

	FSM->GetState(6)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterLongAttack1State);
	FSM->GetState(6)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateLongAttack1State);
	FSM->GetState(6)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitLongAttack1State);

	FSM->GetState(7)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterLongAttack2State);
	FSM->GetState(7)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateLongAttack2State);
	FSM->GetState(7)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitLongAttack2State);

	FSM->GetState(8)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterLongAttack3State);
	FSM->GetState(8)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateLongAttack3State);
	FSM->GetState(8)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitLongAttack3State);

	FSM->GetState(13)->OnEnterState.AddDynamic(this, &AMordathGhost::OnEnterDeathState);
	FSM->GetState(13)->OnUpdateState.AddDynamic(this, &AMordathGhost::UpdateDeathState);
	FSM->GetState(13)->OnExitState.AddDynamic(this, &AMordathGhost::OnExitDeathState);

	FSM->InitState(1);

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
}

void AMordathGhost::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UOverthroneFunctionLibrary::GetPlayerCharacter(this);
	MordathAnimInstance = Cast<UMordathAnimInstance>(SKMComponent->GetAnimInstance());

	const int32& RandomNumber = FMath::RandRange(0, 2);
	switch (RandomNumber)
	{
	case 0:
		CurrentStageData = StageOneData;
	break;

	case 1:
		CurrentStageData = StageTwoData;
	break;

	case 2:
		CurrentStageData = StageThreeData;
	break;

	default:
		CurrentStageData = StageOneData;
	break;
	}

	FSM->Start();

	CurrentStageData->Init();

	ChooseCombo();
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

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->ForwardInput = ForwardInput;
	AnimInstance->RightInput = RightInput;
}

void AMordathGhost::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
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

	if (ChosenCombo->IsAtLastAttack())
	{
		ChooseCombo();
	}

	ChooseMovementDirection();
}

void AMordathGhost::UpdateFollowState()
{
	FacePlayer();

	if (DistanceToPlayer < CurrentStageData->GetMidRangeRadius())
	{
		FSM->PushState("Thinking");
		return;
	}

	// Move towards the player
	if (DistanceToPlayer > CurrentStageData->GetCloseRangeRadius())
	{
		if (!IsDelayingAttack())
		{
			MoveForward();
		}
		else
			FSM->PushState("Thinking");
	}
	else
	{
		CurrentMovementSpeed = 0.0f;
		ForwardInput = 0.0f;
		RightInput = 0.0f;
	}
}

void AMordathGhost::OnExitFollowState()
{
}
#pragma endregion

#pragma region Think
void AMordathGhost::OnEnterThinkState()
{
	MovementComponent->MaxWalkSpeed = CurrentStageData->GetWalkSpeed();

	ChooseMovementDirection();

	ThinkTime = CurrentStageData->ThinkStateData.CalculateThinkTime();

	MordathAnimInstance->bIsThinking = true;
	MordathAnimInstance->bWantsSideStepDash = FMath::RandRange(0, 1);
}

void AMordathGhost::UpdateThinkState()
{
	const float Uptime = FSM->GetActiveStateUptime();

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

void AMordathGhost::UpdateShortAttack1State()
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

void AMordathGhost::UpdateShortAttack2State()
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

void AMordathGhost::UpdateShortAttack3State()
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
	GameState->BossData.CurrentAttackType = LongAttack_1;

	CurrentLongAttackMontage = CurrentStageData->ComboSettings.FarRangeAttackAnim;
	PlayAnimMontage(CurrentStageData->ComboSettings.FarRangeAttackAnim, 1.0f, FName("Anticipation"));
}

void AMordathGhost::UpdateLongAttack1State()
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

void AMordathGhost::UpdateLongAttack2State()
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

void AMordathGhost::UpdateLongAttack3State()
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

	GameState->BossData.OnDeath.Broadcast();

	OnDeath();
}

void AMordathGhost::UpdateDeathState()
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
}

void AMordathGhost::ChooseCombo()
{
}

void AMordathGhost::NextAttack()
{
}

bool AMordathGhost::IsDelayingAttack() const
{
	return TimerManager->IsTimerActive(ChosenCombo->GetAttackDelayTimer());
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
	return CurrentStageData->GetShortAttackDamage();
}

float AMordathGhost::GetLongAttackDamage() const
{
	return CurrentStageData->GetLongAttackDamage();
}

float AMordathGhost::GetSpecialAttackDamage() const
{
	return CurrentStageData->GetSpecialAttackDamage();
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
	return !AnimInstance->Montage_IsPlaying(CurrentAttackData->Attack->AttackMontage);
}

void AMordathGhost::PlayAttackMontage()
{
	PlayAnimMontage(CurrentAttackData->Attack->AttackMontage, 1.0f, FName("Anticipation"));
}

void AMordathGhost::StopAttackMontage()
{
	if (!HasFinishedAttack() && !GameState->IsPlayerDead())
		StopAnimMontage(CurrentAttackData->Attack->AttackMontage);

	CurrentMontageSection = "None";
}

void AMordathGhost::ChooseAttack()
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
			CurrentAttackData = &ChosenCombo->GetCurrentAttackData();

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

void AMordathGhost::EncirclePlayer()
{
}
