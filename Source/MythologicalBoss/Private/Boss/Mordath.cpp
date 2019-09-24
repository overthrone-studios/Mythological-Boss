// Copyright Overthrone Studios 2019

#include "Mordath.h"

#include "OverthroneFunctionLibrary.h"
#include "OverthroneGameInstance.h"
#include "OverthroneGameState.h"
#include "OverthroneHUD.h"
#include "OverthroneEnums.h"

#include "Potions/PotionBase.h"

#include "Misc/MordathStageData.h"

#include "FSM.h"
#include "Log.h"

#include "Boss/BossAIController.h"
#include "Boss/MordathAnimInstance.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TeleportationComponent.h"
#include "Components/DashComponent.h"
#include "Components/FlashIndicatorComponent.h"

#include "Animation/AnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "HUD/MasterHUD.h"
#include "HUD/MainPlayerHUD.h"
#include "HUD/FSMVisualizerHUD.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Engine/StaticMesh.h"

#include "ConstructorHelpers.h"
#include "TimerManager.h"

AMordath::AMordath()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Mordath/Animations/ABP_Mordath.ABP_Mordath_C'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint> BP_HealthPotion(TEXT("Blueprint'/Game/Blueprints/Potions/BP_HealthPotion.BP_HealthPotion'"));

	//if (BP_HealthPotion.Succeeded())
	//	HealthPotion = BP_HealthPotion.Object->GeneratedClass;

	// Get the skeletal mesh to use
	SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/Characters/Mordath/SKM_Mordath.SKM_Mordath'")));

	// Configure our mesh
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -190.0f));
		GetMesh()->SetWorldScale3D(FVector(2.0f));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		if (AnimBP.Succeeded())
			GetMesh()->AnimClass = AnimBP.Class;
		else
			ULog::DebugMessage(ERROR, FString("AnimBP did not succeed."));
	}

	// Create a FSM
	FSM = CreateDefaultSubobject<UFSM>(FName("Boss FSM"));
	FSM->AddState(0, "Idle");
	FSM->AddState(1, "Follow");
	FSM->AddState(2, "Thinking");
	FSM->AddState(3, "Light Attack 1");
	FSM->AddState(4, "Light Attack 2");
	FSM->AddState(5, "Light Attack 3");
	FSM->AddState(6, "Heavy Attack 1");
	FSM->AddState(7, "Heavy Attack 2");
	FSM->AddState(8, "Heavy Attack 3");
	FSM->AddState(9, "Special Attack 1");
	FSM->AddState(10, "Special Attack 2");
	FSM->AddState(11, "Special Attack 3");
	FSM->AddState(12, "Damaged");
	FSM->AddState(13, "Death");
	FSM->AddState(14, "Stunned");
	FSM->AddState(15, "Laugh");
	FSM->AddState(16, "Dash");
	FSM->AddState(17, "Beaten");
	FSM->AddState(18, "Teleport");
	FSM->AddState(19, "Retreat");

	// Bind state events to our functions
	FSM->GetState(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterIdleState);
	FSM->GetState(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateIdleState);
	FSM->GetState(0)->OnExitState.AddDynamic(this, &AMordath::OnExitIdleState);

	FSM->GetState(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFollowState);
	FSM->GetState(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFollowState);
	FSM->GetState(1)->OnExitState.AddDynamic(this, &AMordath::OnExitFollowState);

	FSM->GetState(2)->OnEnterState.AddDynamic(this, &AMordath::OnEnterThinkState);
	FSM->GetState(2)->OnUpdateState.AddDynamic(this, &AMordath::UpdateThinkState);
	FSM->GetState(2)->OnExitState.AddDynamic(this, &AMordath::OnExitThinkState);

	FSM->GetState(3)->OnEnterState.AddDynamic(this, &AMordath::OnEnterShortAttack1State);
	FSM->GetState(3)->OnUpdateState.AddDynamic(this, &AMordath::UpdateShortAttack1State);
	FSM->GetState(3)->OnExitState.AddDynamic(this, &AMordath::OnExitShortAttack1State);

	FSM->GetState(4)->OnEnterState.AddDynamic(this, &AMordath::OnEnterShortAttack2State);
	FSM->GetState(4)->OnUpdateState.AddDynamic(this, &AMordath::UpdateShortAttack2State);
	FSM->GetState(4)->OnExitState.AddDynamic(this, &AMordath::OnExitShortAttack2State);

	FSM->GetState(5)->OnEnterState.AddDynamic(this, &AMordath::OnEnterShortAttack3State);
	FSM->GetState(5)->OnUpdateState.AddDynamic(this, &AMordath::UpdateShortAttack3State);
	FSM->GetState(5)->OnExitState.AddDynamic(this, &AMordath::OnExitShortAttack3State);

	FSM->GetState(6)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLongAttack1State);
	FSM->GetState(6)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLongAttack1State);
	FSM->GetState(6)->OnExitState.AddDynamic(this, &AMordath::OnExitLongAttack1State);

	FSM->GetState(7)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLongAttack2State);
	FSM->GetState(7)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLongAttack2State);
	FSM->GetState(7)->OnExitState.AddDynamic(this, &AMordath::OnExitLongAttack2State);

	FSM->GetState(8)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLongAttack3State);
	FSM->GetState(8)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLongAttack3State);
	FSM->GetState(8)->OnExitState.AddDynamic(this, &AMordath::OnExitLongAttack3State);

	FSM->GetState(12)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDamagedState);
	FSM->GetState(12)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDamagedState);
	FSM->GetState(12)->OnExitState.AddDynamic(this, &AMordath::OnExitDamagedState);

	FSM->GetState(13)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDeathState);
	FSM->GetState(13)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDeathState);
	FSM->GetState(13)->OnExitState.AddDynamic(this, &AMordath::OnExitDeathState);

	FSM->GetState(14)->OnEnterState.AddDynamic(this, &AMordath::OnEnterStunnedState);
	FSM->GetState(14)->OnUpdateState.AddDynamic(this, &AMordath::UpdateStunnedState);
	FSM->GetState(14)->OnExitState.AddDynamic(this, &AMordath::OnExitStunnedState);

	FSM->GetState(15)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLaughState);
	FSM->GetState(15)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLaughState);
	FSM->GetState(15)->OnExitState.AddDynamic(this, &AMordath::OnExitLaughState);

	FSM->GetState(16)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDashState);
	FSM->GetState(16)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDashState);
	FSM->GetState(16)->OnExitState.AddDynamic(this, &AMordath::OnExitDashState);

	FSM->GetState(17)->OnEnterState.AddDynamic(this, &AMordath::OnEnterBeatenState);
	FSM->GetState(17)->OnUpdateState.AddDynamic(this, &AMordath::UpdateBeatenState);
	FSM->GetState(17)->OnExitState.AddDynamic(this, &AMordath::OnExitBeatenState);

	FSM->GetState(18)->OnEnterState.AddDynamic(this, &AMordath::OnEnterTeleportState);
	FSM->GetState(18)->OnUpdateState.AddDynamic(this, &AMordath::UpdateTeleportState);
	FSM->GetState(18)->OnExitState.AddDynamic(this, &AMordath::OnExitTeleportState);

	FSM->GetState(19)->OnEnterState.AddDynamic(this, &AMordath::OnEnterRetreatState);
	FSM->GetState(19)->OnUpdateState.AddDynamic(this, &AMordath::UpdateRetreatState);
	FSM->GetState(19)->OnExitState.AddDynamic(this, &AMordath::OnExitRetreatState);

	FSM->InitState(0);

	// Create a range FSM
	RangeFSM = CreateDefaultSubobject<UFSM>(FName("Range FSM"));
	RangeFSM->AddState(0, "Close");
	RangeFSM->AddState(1, "Mid");
	RangeFSM->AddState(2, "Far");
	RangeFSM->AddState(3, "Super Close");

	RangeFSM->GetState(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterCloseRange);
	RangeFSM->GetState(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateCloseRange);
	RangeFSM->GetState(0)->OnExitState.AddDynamic(this, &AMordath::OnExitCloseRange);

	RangeFSM->GetState(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterMidRange);
	RangeFSM->GetState(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateMidRange);
	RangeFSM->GetState(1)->OnExitState.AddDynamic(this, &AMordath::OnExitMidRange);

	RangeFSM->GetState(2)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFarRange);
	RangeFSM->GetState(2)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFarRange);
	RangeFSM->GetState(2)->OnExitState.AddDynamic(this, &AMordath::OnExitFarRange);

	RangeFSM->GetState(3)->OnEnterState.AddDynamic(this, &AMordath::OnEnterSuperCloseRange);
	RangeFSM->GetState(3)->OnUpdateState.AddDynamic(this, &AMordath::UpdateSuperCloseRange);
	RangeFSM->GetState(3)->OnExitState.AddDynamic(this, &AMordath::OnExitSuperCloseRange);

	RangeFSM->InitState(0);

	// Create a stage FSM
	StageFSM = CreateDefaultSubobject<UFSM>(FName("Stage FSM"));
	StageFSM->AddState(0, "Stage 1");
	StageFSM->AddState(1, "Stage 2");
	StageFSM->AddState(2, "Stage 3");

	StageFSM->GetState(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFirstStage);
	StageFSM->GetState(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFirstStage);
	StageFSM->GetState(0)->OnExitState.AddDynamic(this, &AMordath::OnExitFirstStage);

	StageFSM->GetState(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterSecondStage);
	StageFSM->GetState(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateSecondStage);
	StageFSM->GetState(1)->OnExitState.AddDynamic(this, &AMordath::OnExitSecondStage);

	StageFSM->GetState(2)->OnEnterState.AddDynamic(this, &AMordath::OnEnterThirdStage);
	StageFSM->GetState(2)->OnUpdateState.AddDynamic(this, &AMordath::UpdateThirdStage);
	StageFSM->GetState(2)->OnExitState.AddDynamic(this, &AMordath::OnExitThirdStage);

	StageFSM->InitState(0);

	// Configure capsule component
	GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(185.0f, true);
	GetCapsuleComponent()->SetCapsuleRadius(75.0f, true);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 2.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	// Configure character settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossAIController::StaticClass();

	// Teleportation component
	TeleportationComponent = CreateDefaultSubobject<UTeleportationComponent>(FName("Teleportation Component"));

	// Dash component
	DashComponent = CreateDefaultSubobject<UDashComponent>(FName("Dash Component"));

	// Flash indicator static mesh component
	FlashIndicator = CreateDefaultSubobject<UFlashIndicatorComponent>(FName("Flash Indicator Mesh"));
	FlashIndicator->SetupAttachment(GetMesh(), "spine03_jnt");
	FlashIndicator->SetRelativeLocation(FVector(0.0f));
	FlashIndicator->SetRelativeRotation(FRotator(0.0f));
	FlashIndicator->SetWorldScale3D(FVector(0.0f));
	FlashIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FlashIndicator->SetCollisionProfileName("NoCollision");
	FlashIndicator->bVisible = false;

	const auto SphereMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")));

	if (SphereMesh)
		FlashIndicator->SetStaticMesh(SphereMesh);
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UOverthroneFunctionLibrary::GetPlayerCharacter(this);
	MordathAnimInstance = Cast<UMordathAnimInstance>(SKMComponent->GetAnimInstance());
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("BossFSMVisualizer"));

	// Initialize game instance variables
	GameState->BossData.StartingHealth = HealthComponent->GetDefaultHealth();
	GameState->BossData.Health = HealthComponent->GetCurrentHealth();
	GameState->BossData.SmoothedHealth = HealthComponent->GetCurrentHealth();
	GameState->BossData.OnLowHealth.AddDynamic(this, &AMordath::OnLowHealth);
	GameState->BossData.OnAttackParryed.AddDynamic(this, &AMordath::OnAttackParryed);
	GameState->BossData.OnAttackBlocked.AddDynamic(this, &AMordath::OnAttackBlocked);
	GameState->PlayerData.OnDeath.AddDynamic(this, &AMordath::OnPlayerDeath);
	GameState->BossData.OnEnterFirstStage.AddDynamic(this, &AMordath::OnFirstStageHealth);
	GameState->BossData.OnEnterSecondStage.AddDynamic(this, &AMordath::OnSecondStageHealth);
	GameState->BossData.OnEnterThirdStage.AddDynamic(this, &AMordath::OnThirdStageHealth);
	GameState->Boss = this;
	SendInfo();

	TimerManager->SetTimer(TH_UpdateInfo, this, &AMordath::SendInfo, 0.05f, true);

	// Begin the state machines
	FSM->Start();
	RangeFSM->Start();
	StageFSM->Start();

	MovementComponent->MaxWalkSpeed = GetMovementSpeed();

	CurrentStageData->Init();

	ResetAttackDamage();

	ChooseCombo();

#if !UE_BUILD_SHIPPING
	GetCapsuleComponent()->bHiddenInGame = false;
#else
	GetCapsuleComponent()->bHiddenInGame = true;
#endif
}

void AMordath::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead)
	{
		AnimInstance->MovementSpeed = 0.0f;
		AnimInstance->ForwardInput = ForwardInput;
		AnimInstance->RightInput = RightInput;
		return;
	}
	
	GameState->BossData.Location = CurrentLocation;
	GameState->BossData.LockOnBoneLocation = SKMComponent->GetSocketLocation(LockOnBoneName);
	//GameState->BossData.LockOnLocation = GameState->BossData.CurrentAttackType == LongAttack_1 ? FMath::Lerp(GameState->BossData.LockOnLocation, GameState->BossData.LockOnBoneLocation, 10* DeltaTime) : FMath::Lerp(GameState->BossData.LockOnLocation, CurrentLocation, 5* DeltaTime);

	DistanceToPlayer = GetDistanceToPlayer();
	DirectionToPlayer = GetDirectionToPlayer();

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->ForwardInput = ForwardInput;
	AnimInstance->RightInput = RightInput;

#if !UE_BUILD_SHIPPING
	if (Debug.bShowRaycasts)
	{
		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetSuperCloseRangeRadius(), 32, FColor::Red, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetCloseRangeRadius(), 32, FColor::Orange, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetMidRangeRadius(), 32, FColor::Cyan, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetMidRangeRadius() * 2, 32, FColor::Green, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);
	}

	const int32& TotalMessages = OverthroneHUD->GetDebugMessagesCount();

	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 11, "Boss Forward Input: " + FString::SanitizeFloat(ForwardInput));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 10, "Boss Right Input: " + FString::SanitizeFloat(RightInput));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 9, "Current Montage Section: " + CurrentMontageSection.ToString());
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 8, "Movement Speed: " + FString::SanitizeFloat(CurrentMovementSpeed));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 7, "Distance To Player: " + FString::SanitizeFloat(DistanceToPlayer));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 6, "Direction To Player: " + FString::SanitizeFloat(DirectionToPlayer.Rotation().Yaw));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 5, "Short Attack Damage: " + FString::SanitizeFloat(ShortAttackDamage));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 4, "Long Attack Damage: " + FString::SanitizeFloat(LongAttackDamage));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 3, "Current Attack: " + UOverthroneEnums::MordathAttackTypeToString(GameState->BossData.CurrentAttackType));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 2, "Current Counter: " + UOverthroneEnums::MordathAttackCounterTypeToString(GameState->BossData.CurrentCounterType));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 1, "Lock-on Location Z: " + FString::SanitizeFloat(GameState->BossData.LockOnBoneLocation.Z));
#endif
}

void AMordath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FlashIndicator->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "spine03_jnt");
}

void AMordath::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
}

#pragma region Boss States
#pragma region Idle
void AMordath::OnEnterIdleState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());
}

void AMordath::UpdateIdleState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (GameState->PlayerData.bIsDead)
		return;

	FacePlayer(DefaultRotationSpeed);

	ForwardInput = 0.0f;
	RightInput = 0.0f;

	FSM->PushState("Thinking");
}

void AMordath::OnExitIdleState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
}
#pragma endregion

#pragma region Follow
void AMordath::OnEnterFollowState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->SetMovementMode(MOVE_Walking);
	MovementComponent->MaxWalkSpeed = GetMovementSpeed();

	if (!ChosenCombo)
	{
		#if !UE_BUILD_SHIPPING
		ULog::DebugMessage(ERROR,FString("There are no combos in the list. A crash will occur!"),true);
		#endif

		return;
	}

	if (ChosenCombo->IsAtLastAttack() && !IsWaitingForNewCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboWithDelay();
		else
			ChooseCombo();
	}

	const uint8 bWantsDashForward = FMath::RandRange(0, 1);
	if (bWantsDashForward && !IsDashing() && DistanceToPlayer > CurrentStageData->GetDashDistanceThreshold() && !DashComponent->IsCooldownActive() && IsInFirstStage()) // Todo remove stage 1 check
	{
		if (IsMidRange())
			DashType = Dash_Forward;
		else if (IsCloseRange() || IsSuperCloseRange())
			DashType = Dash_Backward;

		FSM->PushState("Dash");
		return;
	}

	ChooseMovementDirection();
}

void AMordath::UpdateFollowState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (IsTransitioning())
	{
		BossAIController->StopMovement();

		return;
	}

	FacePlayer(DefaultRotationSpeed);

	if (IsWaitingForNewCombo() && DistanceToPlayer < CurrentStageData->GetCloseRangeRadius())
	{
		FSM->PushState("Retreat");
		return;
	}

	if (IsWaitingForNewCombo() && DistanceToPlayer < CurrentStageData->GetMidRangeRadius())
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

void AMordath::OnExitFollowState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
}
#pragma endregion

#pragma region Retreat
void AMordath::OnEnterRetreatState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = CurrentStageData->GetRunSpeed() / 2.0f;

	RetreatTime = CurrentStageData->RetreatStateData.CalculateRetreatTime();

#if !UE_BUILD_SHIPPING
	if (Debug.bLogRetreatTime)
		ULog::Number(RetreatTime, "Retreat Time: ", true);
#endif
}

void AMordath::UpdateRetreatState()
{
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (IsTransitioning())
	{
		BossAIController->StopMovement();

		return;
	}

	FacePlayer(DefaultRotationSpeed);

	if (DistanceToPlayer > CurrentStageData->GetMidRangeRadius())
		FSM->PopState();

	if (IsWaitingForNewCombo() && DistanceToPlayer < CurrentStageData->GetCloseRangeRadius() || Uptime <= RetreatTime)
	{
		MoveForward(-1.0f);
	}
	else
	{
		FSM->PopState();
	}
}

void AMordath::OnExitRetreatState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
}
#pragma endregion  

#pragma region Think
void AMordath::OnEnterThinkState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = CurrentStageData->GetWalkSpeed();

	ChooseMovementDirection();

	ThinkTime = CurrentStageData->ThinkStateData.CalculateThinkTime();

	MordathAnimInstance->bIsThinking = true;
	MordathAnimInstance->bWantsSideStepDash = FMath::RandRange(0, 1);

#if !UE_BUILD_SHIPPING
	if (Debug.bLogThinkTime)
		ULog::Number(ThinkTime, "Think Time: ", true);
#endif
}

void AMordath::UpdateThinkState()
{
	const float Uptime = FSM->GetActiveStateUptime();
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (IsTransitioning())
	{
		BossAIController->StopMovement();

		return;
	}

	FacePlayer(DefaultRotationSpeed);

	if (AnimInstance->AnimTimeRemaining > 0.2f)
		EncirclePlayer();

	if (!IsWaitingForNewCombo() && Uptime > ThinkTime)
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

void AMordath::OnExitThinkState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bIsThinking = false;
}
#pragma endregion 

#pragma region Light Attack 1
void AMordath::OnEnterShortAttack1State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	PlayAttackMontage();
}

void AMordath::UpdateShortAttack1State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayerBasedOnMontageSection(CurrentAttackData->Attack->AttackMontage);
	
	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitShortAttack1State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion

#pragma region Light Attack 2
void AMordath::OnEnterShortAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	PlayAttackMontage();
}

void AMordath::UpdateShortAttack2State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayerBasedOnMontageSection(CurrentAttackData->Attack->AttackMontage);

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitShortAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion

#pragma region Light Attack 3
void AMordath::OnEnterShortAttack3State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	PlayAttackMontage();
}

void AMordath::UpdateShortAttack3State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayerBasedOnMontageSection(CurrentAttackData->Attack->AttackMontage);

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitShortAttack3State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion 

#pragma region Heavy Attack 1
void AMordath::OnEnterLongAttack1State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	GameState->BossData.CurrentAttackType = LongAttack_1;

	CurrentLongAttackMontage = CurrentStageData->ComboSettings.FarRangeAttackAnim;
	PlayAnimMontage(CurrentStageData->ComboSettings.FarRangeAttackAnim, 1.0f, FName("Anticipation"));
}

void AMordath::UpdateLongAttack1State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	CurrentMontageSection = AnimInstance->Montage_GetCurrentSection(CurrentLongAttackMontage);

	if (CurrentMontageSection != "Recovery")
		FacePlayer(DefaultRotationSpeed);
	else
		FacePlayer(0.5f);

	// If attack animation has finished, go back to previous state
	if (!AnimInstance->Montage_IsPlaying(CurrentLongAttackMontage))
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitLongAttack1State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	// Ensure that anim montage has stopped playing when leaving this state
	StopAnimMontage(CurrentLongAttackMontage);

	CurrentLongAttackMontage = nullptr;
}
#pragma endregion

#pragma region Heavy Attack 2
void AMordath::OnEnterLongAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	PlayAttackMontage();
}

void AMordath::UpdateLongAttack2State()
{
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	FacePlayerBasedOnMontageSection(CurrentAttackData->Attack->AttackMontage);

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitLongAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion

#pragma region Heavy Attack 3
void AMordath::OnEnterLongAttack3State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	PlayAttackMontage();
}

void AMordath::UpdateLongAttack3State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayerBasedOnMontageSection(CurrentAttackData->Attack->AttackMontage);

	// If attack animation has finished, go back to previous state
	if (HasFinishedAttack())
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitLongAttack3State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	// Ensure that anim montage has stopped playing when leaving this state
	StopAttackMontage();
}
#pragma endregion

#pragma region Damaged
void AMordath::OnEnterDamagedState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsHit = true;
}

void AMordath::UpdateDamagedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AMordath::OnExitDamagedState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsHit = false;

	if (ChosenCombo)
		NextAttack();
}
#pragma endregion

#pragma region Death
void AMordath::OnEnterDeathState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	bIsDead = true;
	GameState->BossData.bIsDead = true;
	AnimInstance->bIsDead = true;

	GameState->BossData.OnDeath.Broadcast();

	RangeFSM->Stop();
	StageFSM->Stop();

	TimerManager->SetTimer(DeathExpiryTimerHandle, this, &AMordath::DestroySelf, DeathTime);

	OnDeath();
}

void AMordath::UpdateDeathState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->Stop();
}

void AMordath::OnExitDeathState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	bIsDead = false;
	GameState->BossData.bIsDead = false;
	AnimInstance->bIsDead = false;
}
#pragma endregion

#pragma region Stunned
void AMordath::OnEnterStunnedState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	// Reset hit count
	HitCounter = 0;

	StopAttackMontage();

	MordathAnimInstance->bIsStunned = true;
}

void AMordath::UpdateStunnedState()
{
	const float Uptime = FSM->GetActiveStateUptime();
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime > CurrentStageData->Combat.StunDuration)
		FSM->PopState();
}

void AMordath::OnExitStunnedState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	GameState->PlayerData.bParrySucceeded = false;
	MordathAnimInstance->bIsStunned = false;

	if (ChosenCombo)
		NextAttack();
}
#pragma endregion

#pragma region Laugh
void AMordath::OnEnterLaughState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bCanLaugh = true;
}

void AMordath::UpdateLaughState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(),FSM->GetActiveStateUptime());

}

void AMordath::OnExitLaughState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bCanLaugh = false;
}
#pragma endregion

#pragma region Dash
void AMordath::OnEnterDashState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	DashComponent->StartCooldown();

	FacePlayer();

	// Reset hit count
	HitCounter = 0;

	switch (DashType)
	{
	case Dash_Forward:
		MordathAnimInstance->bIsDashingForward = true;
	break;

	case Dash_Backward:
		MordathAnimInstance->bIsDashingBackward = true;
	break;

	default:
		MordathAnimInstance->bIsDashing = true;
	break;
	}
}

void AMordath::UpdateDashState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AMordath::OnExitDashState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bIsDashingForward = false;
	MordathAnimInstance->bIsDashingBackward = false;
}
#pragma endregion

#pragma region Beaten
void AMordath::OnEnterBeatenState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	//const FVector2D PointOnCircle = FMath::RandPointInCircle(100.0f);
	//World->SpawnActor<APotionBase>(HealthPotion, FVector(PointOnCircle.X, PointOnCircle.Y, 170.0f), FRotator(0.0f));

	MordathAnimInstance->bIsBeaten = true;
}

void AMordath::UpdateBeatenState()
{
	const float Uptime = FSM->GetActiveStateUptime();
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	// If the recover time has finished, go back to previous state
	if (Uptime >= CurrentStageData->Combat.InvincibilityTimeAfterDamage)
	{
		FSM->PopState();
	}
}

void AMordath::OnExitBeatenState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bIsBeaten = false;
}
#pragma endregion

#pragma region Teleport
void AMordath::OnEnterTeleportState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bCanTeleport = true;

	TeleportationComponent->GenerateTeleportTime();
	TeleportationComponent->StartCooldown();
}

void AMordath::UpdateTeleportState()
{
	const float Uptime = FSM->GetActiveStateUptime();
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime > TeleportationComponent->GetTeleportTime())
	{
		if (CurrentAttackData->bCanTeleportWithAttack)
			SetActorLocation(TeleportationComponent->FindLocationToTeleport(GameState->PlayerData.Location, GameState->GetTeleportRadius(), GameState->PlayArea));

		FSM->PopState();
	}
}

void AMordath::OnExitTeleportState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bCanTeleport = false;
}
#pragma endregion  
#pragma endregion

#pragma region Boss Ranges
#pragma region Close Range
void AMordath::OnEnterCloseRange()
{
	FSMVisualizer->HighlightState(RangeFSM->GetActiveStateName().ToString());

	GameState->PlayerData.CurrentRange = Close;
}

void AMordath::UpdateCloseRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());

	if (DistanceToPlayer < CurrentStageData->GetSuperCloseRangeRadius())
		RangeFSM->PushState("Super Close");

	if (DistanceToPlayer > CurrentStageData->GetCloseRangeRadius())
		RangeFSM->PushState("Mid");
}

void AMordath::OnExitCloseRange()
{
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());
}
#pragma endregion 

#pragma region Mid Range
void AMordath::OnEnterMidRange()
{
	FSMVisualizer->HighlightState(RangeFSM->GetActiveStateName().ToString());

	CurrentMovementSpeed = GetMovementSpeed();

	GameState->PlayerData.CurrentRange = Mid;
}

void AMordath::UpdateMidRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());

	if (DistanceToPlayer < CurrentStageData->GetCloseRangeRadius())
		RangeFSM->PushState("Close");

	if (DistanceToPlayer > CurrentStageData->GetMidRangeRadius())
		RangeFSM->PushState("Far");
}

void AMordath::OnExitMidRange()
{
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());
}
#pragma endregion 

#pragma region Far Range
void AMordath::OnEnterFarRange()
{
	FSMVisualizer->HighlightState(RangeFSM->GetActiveStateName().ToString());

	CurrentMovementSpeed = GetMovementSpeed();

	GameState->PlayerData.CurrentRange = Far;
}

void AMordath::UpdateFarRange()
{
	const float& Uptime = RangeFSM->GetActiveStateUptime();
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), Uptime);

	if (DistanceToPlayer < CurrentStageData->GetMidRangeRadius())
	{
		RangeFSM->PushState("Mid");
		return;
	}

	if (IsTeleporting())
		return;

	if (IsInFirstStage() && Uptime > CurrentStageData->ComboSettings.FarRangeAttackDelay)
	{
		if (!IsRecovering())
			FSM->PushState("Heavy Attack 1");
	}
	else if ((IsInSecondStage() || IsInThirdStage()) && Uptime > CurrentStageData->ComboSettings.FarRangeAttackDelay)
	{
		const uint8 bWantsLongAttack = FMath::RandRange(0, 1);
		
		if (bWantsLongAttack)
		{
			if (!IsRecovering())
				FSM->PushState("Heavy Attack 1");
		}
		else
		{
			if (CurrentAttackData->bCanTeleportWithAttack && !TeleportationComponent->IsCoolingDown())
			{
				FSM->PopState();
				FSM->PushState("Teleport");
			}
		}
	}
}

void AMordath::OnExitFarRange()
{
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());
}
#pragma endregion 

#pragma region Super Close
void AMordath::OnEnterSuperCloseRange()
{
	FSMVisualizer->HighlightState(RangeFSM->GetActiveStateName().ToString());

	GameState->PlayerData.CurrentRange = SuperClose;

	IncreaseAttackDamage(1.5f);
}

void AMordath::UpdateSuperCloseRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());

	if (RangeFSM->GetActiveStateUptime() > CurrentStageData->GetSuperCloseRangeTime() && (!IsDashing() && !IsAttacking() && !IsRecovering() && !IsStunned()))
	{
		DashType = Dash_Backward;
		FSM->PushState("Dash");
	}

	if (DistanceToPlayer > CurrentStageData->GetSuperCloseRangeRadius())
		RangeFSM->PopState();
}

void AMordath::OnExitSuperCloseRange()
{
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());

	ResetAttackDamage();
}
#pragma endregion 
#pragma endregion

#pragma region Boss Stages
#pragma region Stage 1
void AMordath::OnEnterFirstStage()
{
	FSMVisualizer->HighlightState(StageFSM->GetActiveStateName().ToString());

	CurrentStageData = StageOneData;

	MordathAnimInstance->CurrentStage = Stage_1;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[0];
}

void AMordath::UpdateFirstStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	// Can we enter the second stage?
	if (HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * SecondStageHealth)
	{
		GameState->BossData.OnEnterSecondStage.Broadcast();
		return;
	}

	if (ChosenCombo->IsAtLastAttack() && !IsWaitingForNewCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboWithDelay();
		else
			ChooseCombo();

		return;
	}

	if (CanAttack())
	{
		// Decide which attack to choose
		if (!IsWaitingForNewCombo() && !IsDelayingAttack())
			ChooseAttack();
	}
}

void AMordath::OnExitFirstStage()
{
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());
}
#pragma endregion 

#pragma region Stage 2
void AMordath::OnEnterSecondStage()
{
	FSMVisualizer->HighlightState(StageFSM->GetActiveStateName().ToString());

	CurrentStageData = StageTwoData;

	if (Stage2_Transition)
		PlayAnimMontage(Stage2_Transition);

	MordathAnimInstance->CurrentStage = Stage_2;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];
}

void AMordath::UpdateSecondStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	if (AnimInstance->Montage_IsPlaying(Stage2_Transition))
		return;

	// Can we enter the third stage?
	if (HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * ThirdStageHealth)
	{
		GameState->BossData.OnEnterThirdStage.Broadcast();
		return;
	}

#if !UE_BUILD_SHIPPING
	// Can we enter the first stage?
	if (HealthComponent->GetCurrentHealth() > HealthComponent->GetDefaultHealth() * SecondStageHealth)
	{
		GameState->BossData.OnEnterFirstStage.Broadcast();
		return;
	}
#endif

	if (ChosenCombo->IsAtLastAttack() && !IsWaitingForNewCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboWithDelay();
		else
			ChooseCombo();

		return;
	}

	if (CanAttack() && !IsTeleporting())
	{
		// Decide which attack to choose
		if (!IsWaitingForNewCombo() && !IsDelayingAttack())
			ChooseAttack();
	}
}

void AMordath::OnExitSecondStage()
{
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());
}
#pragma endregion 

#pragma region Stage 3
void AMordath::OnEnterThirdStage()
{
	FSMVisualizer->HighlightState(StageFSM->GetActiveStateName().ToString());

	CurrentStageData = StageThreeData;

	if (Stage3_Transition)
		PlayAnimMontage(Stage3_Transition);

	MordathAnimInstance->CurrentStage = Stage_3;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];
}

void AMordath::UpdateThirdStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	if (AnimInstance->Montage_IsPlaying(Stage3_Transition))
		return;

#if !UE_BUILD_SHIPPING
	// Can we enter the second stage?
	if (HealthComponent->GetCurrentHealth() > HealthComponent->GetDefaultHealth() * ThirdStageHealth)
	{
		GameState->BossData.OnEnterSecondStage.Broadcast();
		return;
	}
#endif

	if (ChosenCombo->IsAtLastAttack() && !IsWaitingForNewCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboWithDelay();
		else
			ChooseCombo();

		return;
	}

	if (CanAttack() && !IsTeleporting())
	{
		// Decide which attack to choose
		if (!IsWaitingForNewCombo() && !IsDelayingAttack())
			ChooseAttack();
	}
}

void AMordath::OnExitThirdStage()
{
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());
}

#pragma endregion   
#pragma endregion

#pragma region Events
void AMordath::OnLowHealth()
{
	ChangeHitboxSize(CurrentStageData->GetAttackRadiusOnLowHealth());
}

void AMordath::OnPlayerDeath()
{
	BossAIController->StopMovement();

	FSM->RemoveAllStatesFromStack();
	FSM->PushState("Laugh");

	RangeFSM->Stop();
	StageFSM->Stop();
}

void AMordath::OnAttackParryed()
{
	if (CurrentAttackData->Attack->CounterType == Parryable && !IsStunned())
	{
		StopAttackMontage();

		FSM->PopState();
		FSM->PushState("Stunned");

		// Shake the camera
		PlayerController->ClientPlayCameraShake(CurrentStageData->GetStunShake().Shake, CurrentStageData->GetStunShake().Intensity);
	}
}

void AMordath::OnAttackBlocked()
{
	if (CurrentAttackData->Attack->CounterType == Blockable && !IsDamaged())
	{
		StopAttackMontage();

		FSM->PopState();
		FSM->PushState("Damaged");
		
		// Shake the camera
		PlayerController->ClientPlayCameraShake(CurrentStageData->GetDamagedShake().Shake, CurrentStageData->GetDamagedShake().Intensity);
	}
}

void AMordath::OnFirstStageHealth()
{
	StageFSM->PushState(0);

	CachedCombos.Empty();
	ChooseCombo();
}

void AMordath::OnSecondStageHealth()
{
	StageFSM->PushState(1);
	StageFSM->PopState(0);

	CachedCombos.Empty();
	ChooseCombo();
}

void AMordath::OnThirdStageHealth()
{
	StageFSM->PushState(2);
	StageFSM->PopState(1);
	StageFSM->PopState(0);

	CachedCombos.Empty();
	ChooseCombo();
}
#pragma endregion

void AMordath::DestroySelf()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());

	Destroy();
}

void AMordath::PlayAttackMontage()
{
	PlayAnimMontage(CurrentAttackData->Attack->AttackMontage, 1.0f, FName("Anticipation"));

	GameState->BossData.CurrentCounterType = CurrentAttackData->Attack->CounterType;
}

void AMordath::StopAttackMontage()
{
	if (!HasFinishedAttack() && !GameState->IsPlayerDead())
		StopAnimMontage(CurrentAttackData->Attack->AttackMontage);

	CurrentMontageSection = "None";
}

void AMordath::UpdateCharacterInfo()
{
	GameState->BossData.Health = HealthComponent->GetCurrentHealth();
	GameState->BossData.SmoothedHealth = HealthComponent->GetSmoothedHealth();
}

void AMordath::BroadcastLowHealth()
{
	GameState->BossData.OnLowHealth.Broadcast();
	bWasLowHealthEventTriggered = true;
}

void AMordath::FinishStun()
{
	FSM->PopState("Stunned");
}

void AMordath::BeginTakeDamage(const float DamageAmount)
{
	CurrentStageData->Combat.RecentDamage = DamageAmount;

	// Shake the camera
	PlayerController->ClientPlayCameraShake(CurrentStageData->GetDamagedShake().Shake, CurrentStageData->GetDamagedShake().Intensity);
}

void AMordath::ApplyDamage(const float DamageAmount)
{
	HitCounter++;

#if !UE_BUILD_SHIPPING
	if (Debug.bLogHits)
		ULog::DebugMessage(INFO, "Hit Count: " + FString::FromInt(HitCounter), true);
#endif

	UpdateDamageValueInMainHUD(DamageAmount);

	// Apply hit stop
	PauseAnimsWithTimer();

	UpdateHealth(DamageAmount);

	// Handled in blueprints
	OnAfterTakeDamage();
}

void AMordath::EndTakeDamage()
{
	// Are we dead?
	if (HealthComponent->GetCurrentHealth() <= 0.0f && FSM->GetActiveStateName() != "Death")
	{
		Die();
	}
}

void AMordath::ChooseCombo()
{
	if (CurrentStageData->ComboSettings.bChooseRandomCombo)
		ComboIndex = FMath::RandRange(0, CachedCombos.Num()-1);

	if (CachedCombos.Num() > 0)
	{
		// Is the combo data asset valid at 'Index'
		if (CachedCombos[ComboIndex])
		{
			ChosenCombo = CachedCombos[ComboIndex];

			#if !UE_BUILD_SHIPPING
			if (Debug.bLogCurrentCombo)
				ULog::DebugMessage(SUCCESS, "Combo " + ChosenCombo->GetName() + " chosen", true);
			#endif

			ChosenCombo->Init();
			CurrentAttackData = &ChosenCombo->GetCurrentAttackData();

			CachedCombos.Remove(ChosenCombo);
		}
		else
		{
			#if !UE_BUILD_SHIPPING
			ULog::DebugMessage(WARNING, FString("Combo asset at index ") + FString::FromInt(ComboIndex) + FString(" is not valid"), true);
			#endif
		}

		MovementComponent->MaxWalkSpeed = GetMovementSpeed();
	}
	else
	{
		ComboIndex = 0;

		CachedCombos = CurrentStageData->ComboSettings.Combos;

		#if !UE_BUILD_SHIPPING
		switch (StageFSM->GetActiveStateID())
		{
		case 0:
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 1 combos", true);
		break;

		case 1:
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 2 combos", true);
		break;

		case 2:
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 3 combos", true);
		break;

		default:
		break;
		}
		#endif

		ChooseCombo();
	}
}

void AMordath::ChooseComboWithDelay()
{
	if (CurrentStageData->ComboSettings.RandomDeviation == 0.0f)
	{
		TimerManager->SetTimer(TH_ComboDelay, this, &AMordath::ChooseCombo, CurrentStageData->ComboSettings.ComboDelayTime);
		return;
	}

	const float Min = CurrentStageData->ComboSettings.ComboDelayTime - CurrentStageData->ComboSettings.RandomDeviation;
	const float Max = CurrentStageData->ComboSettings.ComboDelayTime + CurrentStageData->ComboSettings.RandomDeviation;
	const float NewDelayTime = FMath::FRandRange(Min, Max);
				
	TimerManager->SetTimer(TH_ComboDelay, this, &AMordath::ChooseCombo, NewDelayTime);

	#if !UE_BUILD_SHIPPING
	if (Debug.bLogComboDelayTime)
		ULog::DebugMessage(INFO, "Delaying: " + FString::SanitizeFloat(NewDelayTime) + " before next combo", true);
	#endif

	MovementComponent->MaxWalkSpeed = MovementComponent->MaxWalkSpeed/2.0f;
}

void AMordath::ChooseAttack()
{
	if (IsAttacking())
		return;

	CurrentAttackData = &ChosenCombo->GetCurrentAttackData();

	// Do a flash to indicate what kind of attack this is
	switch (CurrentAttackData->Attack->CounterType)
	{
	case Parryable:
		FlashIndicator->Flash(CurrentStageData->Combat.ParryableFlashColor);

		GameState->BossData.bCanBeParryed = true;
	break;

	case Blockable:
		FlashIndicator->Flash(CurrentStageData->Combat.BlockableFlashColor);

		GameState->BossData.bCanBeParryed = false;
	break;

	case NoCounter:
		FlashIndicator->Flash(CurrentStageData->Combat.NoCounterFlashColor);

		GameState->BossData.bCanBeParryed = false;
	break;

	default:
		GameState->BossData.bCanBeParryed = false;
	break;
	}

	GameState->BossData.CurrentAttackType = CurrentAttackData->Attack->AttackType;
	GameState->BossData.CurrentCounterType = CurrentAttackData->Attack->CounterType;

	// Choose the current attack from the attack data
	switch (CurrentAttackData->Attack->AttackType)
	{
		case ShortAttack_1:
			FSM->PushState("Light Attack 1");
		break;

		case ShortAttack_2:
			FSM->PushState("Light Attack 2");
		break;

		case ShortAttack_3:
			FSM->PushState("Light Attack 3");
		break;

		case LongAttack_1:
			FSM->PushState("Heavy Attack 1");
		break;

		case LongAttack_2:
			FSM->PushState("Heavy Attack 2");
		break;

		case LongAttack_3:
			FSM->PushState("Heavy Attack 3");
		break;

		default:
		break;
	}
}

void AMordath::NextAttack()
{
	if (ChosenCombo->IsDelayEnabled() && !IsDelayingAttack())
	{
		const float Min = FMath::Clamp(ChosenCombo->GetAttackDelayTime() - ChosenCombo->GetDeviation(), 0.0f, 100.0f);
		const float Max = FMath::Clamp(ChosenCombo->GetAttackDelayTime() + ChosenCombo->GetDeviation(), 0.0f, 100.0f + ChosenCombo->GetDeviation());
		const float NewDelay = FMath::RandRange(Min, Max);

		if (NewDelay > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(ChosenCombo->GetAttackDelayTimer(), this, &AMordath::NextAttack, NewDelay);
			MovementComponent->MaxWalkSpeed = MovementComponent->MaxWalkSpeed/2.0f;
		}
		else
		{
			ChosenCombo->NextAttack();
		}

		return;
	}

	ChosenCombo->NextAttack();
}

void AMordath::UpdateDamageValueInMainHUD(const float DamageAmount) const
{
	MainHUD->FadeInDamageValue();
	MainHUD->UpdateDamageValue(DamageAmount);
}

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead
	if (FSM->GetActiveStateName() == "Death" || AnimInstance->bIsHit)
		return DamageAmount;

	BeginTakeDamage(DamageAmount);

	// Apply damage once
	if (!AnimInstance->bIsHit && HitCounter < CurrentStageData->Combat.MaxHitsBeforeInvincibility && !TimerManager->IsTimerActive(TH_Invincibility))
	{
		ApplyDamage(DamageAmount);
	}

	// When we have reached the maximum amount of hits we can tolerate, enable invincibility
	if (HitCounter == CurrentStageData->Combat.MaxHitsBeforeInvincibility && !TimerManager->IsTimerActive(TH_Invincibility))
	{
		// Reset our hits
		HitCounter = 0;

		// Become invincible and set a timer to disable invincibility after 'X' seconds
		EnableInvincibility();
		TimerManager->SetTimer(TH_Invincibility, this, &AMordath::DisableInvincibility, CurrentStageData->Combat.InvincibilityTimeAfterDamage);

		// Cancel our current animation and enter the downed state
		FSM->PushState("Beaten");

		UpdateDamageValueInMainHUD(DamageAmount);

		UpdateHealth(DamageAmount);

		// Handled in blueprints
		OnAfterTakeDamage();
	}

	EndTakeDamage();

	return DamageAmount;
}

void AMordath::ChangeHitboxSize(const float NewRadius)
{
	CurrentStageData->SetAttackRadius(NewRadius);
}

void AMordath::FacePlayer(const float RotationSpeed)
{
	SetActorRotation(FMath::Lerp(GetControlRotation(), FRotator(GetControlRotation().Pitch, DirectionToPlayer.Rotation().Yaw, GetControlRotation().Roll), RotationSpeed * World->DeltaTimeSeconds));
}

void AMordath::FacePlayer()
{
	SetActorRotation(FRotator(GetControlRotation().Pitch, DirectionToPlayer.Rotation().Yaw, GetControlRotation().Roll));
}

void AMordath::FacePlayerBasedOnMontageSection(class UAnimMontage* Montage)
{
	CurrentMontageSection = AnimInstance->Montage_GetCurrentSection(Montage);

	if (CurrentMontageSection == "Anticipation")
	{
		FacePlayer(CurrentAttackData->Attack->AnticipationRotationSpeed);
	}
	else if (CurrentMontageSection == "Pinnacle")
	{
		if (CurrentAttackData->Attack->bSnapToPlayerLocation)
		{
			FVector NewLocation;

			if (CurrentAttackData->Attack->bLerp)
				NewLocation = FMath::Lerp(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * CurrentAttackData->Attack->DistanceFromPlayer, CurrentAttackData->Attack->LerpSpeed * World->DeltaTimeSeconds);
			else
				NewLocation = GameState->PlayerData.Location - GetActorForwardVector() * CurrentAttackData->Attack->DistanceFromPlayer;

			NewLocation.Z = CurrentLocation.Z;
			SetActorLocation(NewLocation);
		}

		FacePlayer();
	}
	else if (CurrentMontageSection == "Contact")
	{
		FacePlayer(CurrentAttackData->Attack->ContactRotationSpeed);
	}
	else if (CurrentMontageSection == "Recovery")
	{
		FacePlayer(CurrentAttackData->Attack->RecoveryRotationSpeed);
	}
	else
		FacePlayer(DefaultRotationSpeed);
}

void AMordath::SendInfo()
{
	GameState->BossData.Health = HealthComponent->GetCurrentHealth();
	GameState->BossData.SmoothedHealth = HealthComponent->GetSmoothedHealth();
}

bool AMordath::IsStunned() const
{
	return FSM->GetActiveStateID() == 14;
}

void AMordath::ChooseMovementDirection()
{
	MoveDirection = FMath::RandRange(0, 1);
}

void AMordath::EncirclePlayer()
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
		if (WantsMoveRight())
		{
			MoveRight();
		}
		else
		{
			MoveRight(-1.0f);
		}
	}
}

void AMordath::ResetMeshScale()
{
	SKMComponent->SetWorldScale3D(FVector(1.3f));
}

bool AMordath::CanAttack() const
{
	return IsCloseRange() && !IsRecovering() && !IsAttacking() && !IsDashing() && !IsTransitioning() && !IsStunned() && !IsDamaged();
}

void AMordath::ResetAttackDamage()
{
	ShortAttackDamage = CurrentStageData->GetShortAttackDamage();
	LongAttackDamage = CurrentStageData->GetLongAttackDamage();
	SpecialAttackDamage = CurrentStageData->GetSpecialAttackDamage();
}

void AMordath::IncreaseAttackDamage(const float& Multiplier)
{
	ShortAttackDamage *= Multiplier;
	LongAttackDamage *= Multiplier;
	SpecialAttackDamage *= Multiplier;
}

float AMordath::GetDistanceToPlayer() const
{
	const float Distance = FVector::Dist(CurrentLocation, GameState->PlayerData.Location);

	#if !UE_BUILD_SHIPPING
	if (Debug.bLogDistance)
		ULog::DebugMessage(INFO, FString("Distance: ") + FString::SanitizeFloat(Distance), true);
	#endif

	return Distance;
}

FVector AMordath::GetDirectionToPlayer() const
{
	FVector Direction = GameState->PlayerData.Location - CurrentLocation;
	Direction.Normalize();

	#if !UE_BUILD_SHIPPING
	if (Debug.bLogDirection)
		ULog::DebugMessage(INFO, FString("Direction: ") + Direction.ToString(), true);
	#endif

	return Direction;
}

void AMordath::Die()
{
	Super::Die();

	MordathAnimInstance->LeaveAllStates();

	BossAIController->StopMovement();
	MovementComponent->SetMovementMode(MOVE_None);
	MovementComponent->DisableMovement();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FSM->RemoveAllStatesFromStack();
	FSM->PushState("Death");
}

void AMordath::PauseAnimsWithTimer()
{
	if (CurrentStageData->IsHitStopEnabled())
	{
		PauseAnims();
		TimerManager->SetTimer(HitStopTimerHandle, this, &AMordath::UnPauseAnims, CurrentStageData->GetHitStopTime());
	}
}

bool AMordath::IsAttacking() const
{
	return IsShortAttacking() || IsLongAttacking() || IsSpecialAttacking();
}

bool AMordath::IsShortAttacking() const
{
	return FSM->GetActiveStateID() == 3 || FSM->GetActiveStateID() == 4 || FSM->GetActiveStateID() == 5;
}

bool AMordath::IsLongAttacking() const
{
	return FSM->GetActiveStateID() == 6 || FSM->GetActiveStateID() == 7 || FSM->GetActiveStateID() == 8;
}

bool AMordath::IsSpecialAttacking() const
{
	return FSM->GetActiveStateID() == 9 || FSM->GetActiveStateID() == 10 || FSM->GetActiveStateID() == 11;
}

bool AMordath::IsInFirstStage() const
{
	return StageFSM->GetActiveStateID() == 0;
}

bool AMordath::IsInSecondStage() const
{
	return StageFSM->GetActiveStateID() == 1;
}

bool AMordath::IsInThirdStage() const
{
	return StageFSM->GetActiveStateID() == 2;
}

bool AMordath::IsSuperCloseRange() const
{
	return RangeFSM->GetActiveStateID() == 3;
}

bool AMordath::IsCloseRange() const
{
	return RangeFSM->GetActiveStateID() == 0 || RangeFSM->GetActiveStateID() == 3;
}

bool AMordath::IsMidRange() const
{
	return RangeFSM->GetActiveStateID() == 1;
}

bool AMordath::IsFarRange() const
{
	return RangeFSM->GetActiveStateID() == 2;
}

bool AMordath::InInvincibleState() const
{
	return FSM->GetActiveStateID() == 1 || FSM->GetActiveStateID() == 2 || FSM->GetActiveStateID() == 3 || 
		   FSM->GetActiveStateID() == 4 || FSM->GetActiveStateID() == 5 || FSM->GetActiveStateID() == 6 || 
		   FSM->GetActiveStateID() == 7 || FSM->GetActiveStateID() == 8 || FSM->GetActiveStateID() == 9 ||
		   FSM->GetActiveStateID() == 10 || FSM->GetActiveStateID() == 11 || FSM->GetActiveStateID() == 19;
}

bool AMordath::IsWaitingForNewCombo() const
{
	return TimerManager->IsTimerActive(TH_ComboDelay);
}

bool AMordath::IsDelayingAttack() const
{
	return TimerManager->IsTimerActive(ChosenCombo->GetAttackDelayTimer());
}

bool AMordath::IsDashing() const
{
	return FSM->GetActiveStateID() == 16;
}

bool AMordath::IsDamaged() const
{
	return FSM->GetActiveStateID() == 12;
}

bool AMordath::WantsMoveRight() const
{
	return MoveDirection == 1;
}

bool AMordath::IsRecovering() const
{
	return FSM->GetActiveStateID() == 17;
}

bool AMordath::HasFinishedAttack() const
{
	return !AnimInstance->Montage_IsPlaying(CurrentAttackData->Attack->AttackMontage);
}

bool AMordath::IsTransitioning() const
{
	return AnimInstance->Montage_IsPlaying(Stage2_Transition) || AnimInstance->Montage_IsPlaying(Stage3_Transition);
}

bool AMordath::IsTeleporting() const
{
	return FSM->GetActiveStateID() == 18;
}

void AMordath::MoveForward(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * DirectionToPlayer);
	ForwardInput = Scale;
	RightInput = 0.0f;
}

void AMordath::MoveRight(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * GetActorRightVector());
	ForwardInput = 0.0f;
	RightInput = Scale;
}

float AMordath::GetMovementSpeed() const
{
	switch (RangeFSM->GetActiveStateID())
	{
	case 0 /*Close*/:
		return CurrentStageData->GetWalkSpeed();

	case 1 /*Mid*/:
		return CurrentStageData->GetRunSpeed();

	case 2 /*Far*/:
		return CurrentStageData->GetRunSpeed();

	default:
		return CurrentStageData->GetWalkSpeed();
	}
}

float AMordath::GetShortAttackDamage() const
{
	return ShortAttackDamage;
}

float AMordath::GetLongAttackDamage() const
{
	return LongAttackDamage;
}

float AMordath::GetSpecialAttackDamage() const
{
	return SpecialAttackDamage;
}

float AMordath::GetAttackRadius() const
{
	return CurrentStageData->GetAttackRadius();
}

float AMordath::GetRecentDamage() const
{
	return CurrentStageData->GetRecentDamage();
}

void AMordath::EnterStage(const EBossStage InStage)
{
	switch (InStage)
	{
	case Stage_1:
		SetHealth(HealthComponent->GetDefaultHealth());
	break;

	case Stage_2:
		SetHealth(HealthComponent->GetDefaultHealth() * SecondStageHealth);
	break;
		
	case Stage_3:
		SetHealth(HealthComponent->GetDefaultHealth() * ThirdStageHealth);
	break;
	}
}

void AMordath::AddDebugMessages()
{
	const float YPadding = 5.0f;

	OverthroneHUD->AddOnScreenDebugMessage("Boss", FColor::White, YPadding, 1.1f);
	OverthroneHUD->AddOnScreenDebugMessage("Boss Forward Input: ", FColor::Green, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Boss Right Input: ", FColor::Green, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Current Montage Section: ", FColor::Yellow, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Movement Speed: ", FColor::Yellow, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Distance To Player: ", FColor::Cyan, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Direction To Player: ", FColor::Cyan, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Short Attack Damage: ", FColor::Green, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Long Attack Damage: ", FColor::Green, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Current Attack: ", FColor::Yellow, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Current Counter: ", FColor::Yellow, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Lock-on Location: ", FColor::Green, YPadding);
}
