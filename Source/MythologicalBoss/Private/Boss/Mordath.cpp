// Copyright Overthrone Studios 2019

#include "Mordath.h"
#include "Public/OverthroneHUD.h"
#include "Public/OverthroneGameInstance.h"
#include "BossAIController.h"
#include "Boss/MordathAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "HUD/MasterHUD.h"
#include "HUD/MainPlayerHUD.h"
#include "HUD/FSMVisualizerHUD.h"
#include "TimerManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "FSM.h"
#include "Log.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Components/TimelineComponent.h"

AMordath::AMordath()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Boss/Animations/AnimBP_Boss.AnimBP_Boss_C'"));

	// Get the skeletal mesh to use
	SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/Characters/Boss/SKM_Boss.SKM_Boss'")));

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
	BossStateMachine = CreateDefaultSubobject<UFSM>(FName("Boss FSM"));
	BossStateMachine->AddState(0, "Idle");
	BossStateMachine->AddState(1, "Follow");
	BossStateMachine->AddState(3, "Light Attack 1");
	BossStateMachine->AddState(4, "Light Attack 2");
	BossStateMachine->AddState(5, "Light Attack 3");
	BossStateMachine->AddState(6, "Heavy Attack 1");
	BossStateMachine->AddState(7, "Heavy Attack 2");
	BossStateMachine->AddState(8, "Heavy Attack 3");
	BossStateMachine->AddState(9, "Special Attack 1");
	BossStateMachine->AddState(10, "Special Attack 2");
	BossStateMachine->AddState(11, "Special Attack 3");
	BossStateMachine->AddState(12, "Damaged");
	BossStateMachine->AddState(13, "Death");
	BossStateMachine->AddState(14, "Stunned");
	BossStateMachine->AddState(15, "Laugh");
	BossStateMachine->AddState(16, "DashToJump");


	// Bind state events to our functions
	BossStateMachine->GetState(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterIdleState);
	BossStateMachine->GetState(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateIdleState);
	BossStateMachine->GetState(0)->OnExitState.AddDynamic(this, &AMordath::OnExitIdleState);

	BossStateMachine->GetState(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFollowState);
	BossStateMachine->GetState(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFollowState);
	BossStateMachine->GetState(1)->OnExitState.AddDynamic(this, &AMordath::OnExitFollowState);

	BossStateMachine->GetState(3)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack1State);
	BossStateMachine->GetState(3)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack1State);
	BossStateMachine->GetState(3)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack1State);

	BossStateMachine->GetState(4)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack2State);
	BossStateMachine->GetState(4)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack2State);
	BossStateMachine->GetState(4)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack2State);

	BossStateMachine->GetState(5)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack3State);
	BossStateMachine->GetState(5)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack3State);
	BossStateMachine->GetState(5)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack3State);

	BossStateMachine->GetState(6)->OnEnterState.AddDynamic(this, &AMordath::OnEnterHeavyAttack1State);
	BossStateMachine->GetState(6)->OnUpdateState.AddDynamic(this, &AMordath::UpdateHeavyAttack1State);
	BossStateMachine->GetState(6)->OnExitState.AddDynamic(this, &AMordath::OnExitHeavyAttack1State);

	BossStateMachine->GetState(7)->OnEnterState.AddDynamic(this, &AMordath::OnEnterHeavyAttack2State);
	BossStateMachine->GetState(7)->OnUpdateState.AddDynamic(this, &AMordath::UpdateHeavyAttack2State);
	BossStateMachine->GetState(7)->OnExitState.AddDynamic(this, &AMordath::OnExitHeavyAttack2State);

	BossStateMachine->GetState(8)->OnEnterState.AddDynamic(this, &AMordath::OnEnterHeavyAttack3State);
	BossStateMachine->GetState(8)->OnUpdateState.AddDynamic(this, &AMordath::UpdateHeavyAttack3State);
	BossStateMachine->GetState(8)->OnExitState.AddDynamic(this, &AMordath::OnExitHeavyAttack3State);

	BossStateMachine->GetState(12)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDamagedState);
	BossStateMachine->GetState(12)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDamagedState);
	BossStateMachine->GetState(12)->OnExitState.AddDynamic(this, &AMordath::OnExitDamagedState);

	BossStateMachine->GetState(13)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDeathState);
	BossStateMachine->GetState(13)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDeathState);
	BossStateMachine->GetState(13)->OnExitState.AddDynamic(this, &AMordath::OnExitDeathState);

	BossStateMachine->GetState(14)->OnEnterState.AddDynamic(this, &AMordath::OnEnterStunnedState);
	BossStateMachine->GetState(14)->OnUpdateState.AddDynamic(this, &AMordath::UpdateStunnedState);
	BossStateMachine->GetState(14)->OnExitState.AddDynamic(this, &AMordath::OnExitStunnedState);

	BossStateMachine->GetState(15)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLaughState);
	BossStateMachine->GetState(15)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLaughState);
	BossStateMachine->GetState(15)->OnExitState.AddDynamic(this, &AMordath::OnExitLaughState);

	BossStateMachine->GetState(16)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDashToJumpState);
	BossStateMachine->GetState(16)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDashToJumpState);
	BossStateMachine->GetState(16)->OnExitState.AddDynamic(this, &AMordath::OnExitDashToJumpState);

	BossStateMachine->InitState(0);

	// Assign the behaviour tree
	BossBT = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/AI/BT_Boss.BT_Boss'")));

	// Configure capsule component
	GetCapsuleComponent()->SetCollisionProfileName(FName("BlockAll"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(185.0f, true);
	GetCapsuleComponent()->SetCapsuleRadius(75.0f, true);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 2.0f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Configure character settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossAIController::StaticClass();
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Timeline components
	JumpAttackTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(FName("Jump Attack Timeline"));
	DashTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(FName("Dash Timeline"));

	bCanBeDamaged = true;

	// Configure bezier curves
	JumpAttack_Bezier.PlaybackSpeed = 2.0f;
	JumpAttack_Bezier.CurveHeight = 1000.0f;
	JumpAttack_Bezier.EndPointOffsetDistance = 100.0f;

	Dash_Bezier.PlaybackSpeed = 2.0f;
	Dash_Bezier.CurveHeight = 190.0f;
}

void AMordath::InitJumpAttackTimeline()
{
	// Timeline Initialization
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, "DoJumpAttack");

	if (JumpAttack_Bezier.Curve)
	{
		JumpAttackTimelineComponent = NewObject<UTimelineComponent>(this, FName("Jump Attack Timeline"));
		JumpAttackTimelineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		JumpAttackTimelineComponent->SetPropertySetObject(this);
		JumpAttackTimelineComponent->SetLooping(false);
		JumpAttackTimelineComponent->SetPlaybackPosition(0.0f, false, false);
		JumpAttackTimelineComponent->SetPlayRate(JumpAttack_Bezier.PlaybackSpeed);
		JumpAttackTimelineComponent->AddInterpFloat(JumpAttack_Bezier.Curve, TimelineCallback);
		JumpAttackTimelineComponent->SetTimelineLength(JumpAttack_Bezier.Curve->FloatCurve.Keys[JumpAttack_Bezier.Curve->FloatCurve.Keys.Num() - 1].Time);
		JumpAttackTimelineComponent->SetTimelineLengthMode(TL_TimelineLength);
		JumpAttackTimelineComponent->RegisterComponent();
	}
	else
	{
		ULog::DebugMessage(ERROR, "Failed to initialize the jump attack timeline. Jump attack curve is missing!", true);
	}
}

void AMordath::InitDashTimeline()
{
	// Timeline Initialization
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, "DoDash");
	TimelineFinishedCallback.BindUFunction(this, "BeginJumpAttack");

	if (Dash_Bezier.Curve)
	{
		DashTimelineComponent = NewObject<UTimelineComponent>(this, FName("Dash Timeline"));
		DashTimelineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		DashTimelineComponent->SetPropertySetObject(this);
		DashTimelineComponent->SetLooping(false);
		DashTimelineComponent->SetPlaybackPosition(0.0f, false, false);
		DashTimelineComponent->SetPlayRate(Dash_Bezier.PlaybackSpeed);
		DashTimelineComponent->AddInterpFloat(Dash_Bezier.Curve, TimelineCallback);
		DashTimelineComponent->SetTimelineFinishedFunc(TimelineFinishedCallback);
		DashTimelineComponent->SetTimelineLength(Dash_Bezier.Curve->FloatCurve.Keys[Dash_Bezier.Curve->FloatCurve.Keys.Num() - 1].Time);
		DashTimelineComponent->SetTimelineLengthMode(TL_TimelineLength);
		DashTimelineComponent->RegisterComponent();
	}
	else
	{
		ULog::DebugMessage(ERROR, "Failed to initialize the dash timeline. Dash curve is missing!", true);
	}
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	InitJumpAttackTimeline();
	InitDashTimeline();

	bCanBeDamaged = true;

	// Cache the world object
	World = GetWorld();

	// Cache the movement component
	MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = WalkSpeed;

	Health = StartingHealth;

	// Cache the Combos array to use for randomization
	CachedCombos = ComboSettings.Combos;

	// Cache the Overthrone HUD
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	// Cache player character for knowing where it is
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	// Cache the main HUD
	MainHUD = Cast<UMainPlayerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("MainHUD"));

	// Cache our anim instance
	AnimInstance = Cast<UMordathAnimInstance>(GetMesh()->GetAnimInstance());

	// Cache our game instance
	GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->BossStartingHealth = StartingHealth;
	GameInstance->OnPlayerDeath.AddDynamic(this, &AMordath::OnPlayerDeath);
	GameInstance->Boss = this;
	SendInfo();

	// Cache the FSM Visualizer HUD
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("BossFSMVisualizer"));

	// Begin the state machine
	BossStateMachine->Start();

	GetWorld()->GetTimerManager().SetTimer(UpdateInfoTimerHandle, this, &AMordath::SendInfo, 0.05f, true);

	ChooseCombo();
}

void AMordath::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameInstance->bParrySucceeded && BossStateMachine->GetActiveStateID() != 14 /*Stunned*/)
		BossStateMachine->PushState("Stunned");

	//// If we are in close range
	//const bool bCloseRange = GetDistanceToPlayer() <= AcceptanceRadius + MidRangeRadius/2.0f;
	//const bool bMidRange = GetDistanceToPlayer() > AcceptanceRadius && GetDistanceToPlayer() < AcceptanceRadius + MidRangeRadius;
	//const bool bFarRange = GetDistanceToPlayer() > AcceptanceRadius + MidRangeRadius && GetDistanceToPlayer() < AcceptanceRadius + FarRangeRadius;

	//if (bCloseRange)
	//	ULog::DebugMessage(INFO, "Close range", true);
	//else if (bMidRange)
	//	ULog::DebugMessage(INFO, "Mid range", true);
	//else if (bFarRange)
	//	ULog::DebugMessage(INFO, "Far range", true);

	//ULog::DebugMessage(INFO, BossStateMachine->GetActiveStateName().ToString(), true);
}

void AMordath::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
}

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!AnimInstance->bIsHit && !GetWorldTimerManager().IsTimerActive(InvincibilityTimerHandle))
	{
		HitCounter++;

		// Go to damaged state if we are not stunned
		if (BossStateMachine->GetActiveStateName() != "Stunned")
		{
			BossStateMachine->PopState();
			BossStateMachine->PushState("Damaged");
		}

		Health = FMath::Clamp(Health - DamageAmount, 0.0f, StartingHealth);
	}

	if (HitCounter >= MaxHitsBeforeInvincibility && !GetWorldTimerManager().IsTimerActive(InvincibilityTimerHandle))
	{
		HitCounter = 0;

		EnableInvincibility();

		GetWorldTimerManager().SetTimer(InvincibilityTimerHandle, this, &AMordath::DisableInvincibility, InvincibilityTimeAfterDamage);

		BossStateMachine->PopState();
		BossStateMachine->PushState("Damaged");

		Health = FMath::Clamp(Health - DamageAmount, 0.0f, StartingHealth);
	}

	if (Health <= 0.0f)
	{
		bCanBeDamaged = false;
		BossStateMachine->PushState("Death");
	}

	return DamageAmount;
}

FRotator AMordath::FacePlayer()
{
	const FVector Target = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), PlayerCharacter->GetActorLocation());
	const FRotator SmoothedRotation = FMath::RInterpTo(GetControlRotation(), Target.Rotation(), World->DeltaTimeSeconds, 20.0f);

	const FRotator NewRotation = FRotator(GetControlRotation().Pitch, SmoothedRotation.Yaw, GetControlRotation().Roll);

	SetActorRotation(NewRotation);

	return Target.Rotation();
}

void AMordath::SendInfo()
{
	GameInstance->BossHealth = Health;
	GameInstance->BossLocation = GetActorLocation();
}

#pragma region Idle
void AMordath::OnEnterIdleState()
{

}

void AMordath::UpdateIdleState()
{
	if (GameInstance->bIsPlayerDead)
		return;

	ULog::DebugMessage(INFO, "Idle state", true);

	FacePlayer();

	if (GetDistanceToPlayer() > AcceptanceRadius)
		BossStateMachine->PushState("Follow");
}

void AMordath::OnExitIdleState()
{
}
#pragma endregion

#pragma region Follow
void AMordath::OnEnterFollowState()
{
	MovementComponent->SetMovementMode(MOVE_Walking);

	if (ChosenCombo->IsAtLastAttack() && !GetWorldTimerManager().IsTimerActive(ComboDelayTimerHandle))
	{
		if (ComboSettings.bDelayBetweenCombo)
			ChooseComboWithDelay();
		else
			ChooseCombo();
	}
}

void AMordath::UpdateFollowState()
{
	// Check for destructible objects and destroy them
	if (ShouldDestroyDestructibleObjects())
	{
		BossAIController->StopMovement();
		BossStateMachine->PushState(3);
		return;
	}

	AddMovementInput(GetDirectionToPlayer());
	SetActorRotation(FRotator(GetControlRotation().Pitch, GetDirectionToPlayer().Rotation().Yaw, GetControlRotation().Roll));

	// If we are in close range
	const bool bCloseRange = GetDistanceToPlayer() <= AcceptanceRadius + MidRangeRadius/2.0f;
	const bool bMidRange = GetDistanceToPlayer() > AcceptanceRadius && GetDistanceToPlayer() < AcceptanceRadius + MidRangeRadius;
	const bool bFarRange = GetDistanceToPlayer() > AcceptanceRadius + MidRangeRadius && GetDistanceToPlayer() < AcceptanceRadius + FarRangeRadius;
	if (bCloseRange)
	{
		ChooseAttack();
	}
	else if (bMidRange)
	{
		// Do dash (to left or right)
		if (!GetWorldTimerManager().IsTimerActive(DashCooldownTimerHandle))
		{
			BeginJumpAttackWithDash();
		}
	}
	else if (bFarRange)
	{
		// Do jump attack
		if (!GetWorldTimerManager().IsTimerActive(JumpAttackCooldownTimerHandle))
		{
			BeginJumpAttack();
		}
	}
}

void AMordath::OnExitFollowState()
{
}
#pragma endregion 

#pragma region Light Attack 1
void AMordath::OnEnterLightAttack1State()
{
	AnimInstance->bAcceptLightAttack = true;
}

void AMordath::UpdateLightAttack1State()
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.4f)
	{
		ChosenCombo->NextAttack();

		BossStateMachine->PopState();
	}
}

void AMordath::OnExitLightAttack1State()
{
	AnimInstance->bAcceptLightAttack = false;
}
#pragma endregion

#pragma region Light Attack 2
void AMordath::OnEnterLightAttack2State()
{
	AnimInstance->bAcceptSecondLightAttack = true;
}

void AMordath::UpdateLightAttack2State()
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.7f)
	{
		ChosenCombo->NextAttack();

		BossStateMachine->PopState();
	}
}

void AMordath::OnExitLightAttack2State()
{
	AnimInstance->bAcceptSecondLightAttack = false;
}
#pragma endregion

#pragma region Light Attack 3
void AMordath::OnEnterLightAttack3State()
{
	AnimInstance->bAcceptThirdLightAttack = true;
}

void AMordath::UpdateLightAttack3State()
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.2f)
	{
		ChosenCombo->NextAttack();

		BossStateMachine->PopState();
	}
}

void AMordath::OnExitLightAttack3State()
{
	AnimInstance->bAcceptThirdLightAttack = false;
}
#pragma endregion 

#pragma region Heavy Attack 1
void AMordath::OnEnterHeavyAttack1State()
{
	AnimInstance->bAcceptHeavyAttack = true;
}

void AMordath::UpdateHeavyAttack1State()
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.2f)
	{
		ChosenCombo->NextAttack();

		BossStateMachine->PopState();
	}
}

void AMordath::OnExitHeavyAttack1State()
{
	AnimInstance->bAcceptHeavyAttack = false;
}
#pragma endregion

#pragma region Heavy Attack 2
void AMordath::OnEnterHeavyAttack2State()
{
	AnimInstance->bAcceptSecondHeavyAttack = true;
}

void AMordath::UpdateHeavyAttack2State()
{
	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.5f)
	{
		ChosenCombo->NextAttack();

		BossStateMachine->PopState();
	}
}

void AMordath::OnExitHeavyAttack2State()
{
	AnimInstance->bAcceptSecondHeavyAttack = false;

	GetWorldTimerManager().SetTimer(JumpAttackCooldownTimerHandle, this, &AMordath::FinishCooldown, CombatSettings.JumpAttackCooldown);
}
#pragma endregion

#pragma region Heavy Attack 3
void AMordath::OnEnterHeavyAttack3State()
{
	AnimInstance->bAcceptThirdHeavyAttack = true;
}

void AMordath::UpdateHeavyAttack3State()
{
	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.3f)
	{
		ChosenCombo->NextAttack();

		BossStateMachine->PopState();
	}
}

void AMordath::OnExitHeavyAttack3State()
{
	AnimInstance->bAcceptThirdHeavyAttack = false;
}
#pragma endregion

#pragma region Damaged
void AMordath::OnEnterDamagedState()
{
	AnimInstance->bIsHit = true;
}

void AMordath::UpdateDamagedState()
{
	// If damaged animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.4f)
		BossStateMachine->PopState();
}

void AMordath::OnExitDamagedState()
{
	AnimInstance->bIsHit = false;

	ChosenCombo->NextAttack();
}
#pragma endregion

#pragma region Death
void AMordath::OnEnterDeathState()
{
	AnimInstance->bIsDead = true;

	BossAIController->StopBT();

	GameInstance->OnBossDeath.Broadcast();

	GetWorldTimerManager().SetTimer(DeathExpiryTimerHandle, this, &AMordath::DestroySelf, DeathTime);
}

void AMordath::UpdateDeathState()
{
}

void AMordath::OnExitDeathState()
{
	AnimInstance->bIsDead = false;
}
#pragma endregion

#pragma region Stunned
void AMordath::OnEnterStunnedState()
{
	AnimInstance->bIsStunned = true;

	GetWorldTimerManager().SetTimer(StunExpiryTimerHandle, this, &AMordath::FinishStun, CombatSettings.StunDuration);
}

void AMordath::UpdateStunnedState()
{
	// If stun animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
		AnimInstance->bIsStunned = false;
}

void AMordath::OnExitStunnedState()
{
	AnimInstance->bIsStunned = false;
}
#pragma endregion

#pragma region Laugh
void AMordath::OnEnterLaughState()
{
	AnimInstance->bCanLaugh = true;
}

void AMordath::UpdateLaughState()
{
}

void AMordath::OnExitLaughState()
{
	AnimInstance->bCanLaugh = false;
}
#pragma endregion

#pragma region Dash to Jump
void AMordath::OnEnterDashToJumpState()
{
}

void AMordath::UpdateDashToJumpState()
{
	FacePlayer();
}

void AMordath::OnExitDashToJumpState()
{
	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &AMordath::FinishCooldown, CombatSettings.DashCooldown);
}
#pragma endregion

void AMordath::OnPlayerDeath()
{
	MovementComponent->DisableMovement();

	BossStateMachine->RemoveAllStatesFromStackExceptActive();

	BossStateMachine->PushState("Laugh");
}

void AMordath::DestroySelf()
{
	MovementComponent->DisableMovement();

	Destroy();
}

void AMordath::FinishCooldown()
{
	ULog::DebugMessage(SUCCESS, "Cooldown finished", true);
}

void AMordath::FinishStun()
{
	BossStateMachine->PopState();
}

bool AMordath::ShouldDestroyDestructibleObjects()
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);

	const FVector Start = GetActorLocation() - FVector(0.0f, 0.0f, 100);
	FVector End = GetActorLocation() + GetActorForwardVector() * BoxDetectionDistance;
	End.Z = Start.Z;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1, 0, 3.0f);

	return GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, CollisionObjectQueryParams);
}

void AMordath::ChooseCombo()
{
	if (ComboSettings.bChooseRandomCombo)
		ComboIndex = FMath::RandRange(0, CachedCombos.Num()-1);

	if (CachedCombos.Num() > 0)
	{
		// If the combo data asset is valid at 'Index'
		if (CachedCombos[ComboIndex])
		{
			ChosenCombo = CachedCombos[ComboIndex];
			ULog::DebugMessage(INFO, "Combo " + ChosenCombo->GetName() + " chosen", true);
			ChosenCombo->Init();

			CachedCombos.Remove(ChosenCombo);
		}
		else
		{
			ULog::DebugMessage(WARNING, FString("Combo asset at index ") + FString::FromInt(ComboIndex) + FString(" is not valid"), true);
		}

		MovementComponent->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		ComboIndex = 0;

		CachedCombos = ComboSettings.Combos;
		ChooseCombo();
	}
}

void AMordath::ChooseComboWithDelay()
{
	if (ComboSettings.RandomDeviation == 0.0f)
	{
		GetWorldTimerManager().SetTimer(ComboDelayTimerHandle, this, &AMordath::ChooseCombo, ComboSettings.ComboDelayTime);
		return;
	}

	const float Min = ComboSettings.ComboDelayTime - ComboSettings.RandomDeviation;
	const float Max = ComboSettings.ComboDelayTime + ComboSettings.RandomDeviation;
	const float NewDelayTime = FMath::FRandRange(Min, Max);
				
	GetWorldTimerManager().SetTimer(ComboDelayTimerHandle, this, &AMordath::ChooseCombo, NewDelayTime);

	ULog::DebugMessage(INFO, "New Delay time: " + FString::SanitizeFloat(NewDelayTime), true);

	MovementComponent->MaxWalkSpeed = WalkSpeed/2;
}

void AMordath::ChooseAttack()
{
	MovementComponent->SetMovementMode(MOVE_None);

	switch (ChosenCombo->CurrentAttack)
	{
		case LightAttack_1:
			BossStateMachine->PushState("Light Attack 1");
		break;

		case LightAttack_2:
			BossStateMachine->PushState("Light Attack 2");
		break;

		case LightAttack_3:
			BossStateMachine->PushState("Light Attack 3");
		break;

		case HeavyAttack_1:
			BossStateMachine->PushState("Heavy Attack 1");
		break;

		case HeavyAttack_2:
			BossStateMachine->PushState("Heavy Attack 2");
		break;

		case HeavyAttack_3:
			BossStateMachine->PushState("Heavy Attack 3");
		break;

		default:
		break;
	}
}

void AMordath::EnableInvincibility()
{
	bCanBeDamaged = false;
}

void AMordath::DisableInvincibility()
{
	bCanBeDamaged = true;
}

void AMordath::BeginJumpAttack()
{
	BossStateMachine->PopState("DashToJump");
	BossStateMachine->PushState("Heavy Attack 2");

	// Create the main points of the bezier curve
	FVector Point = PlayerCharacter->GetActorLocation() + GetDirectionToPlayer() * -(GetDistanceToPlayer() / 2.0f);
	Point.Z = JumpAttack_Bezier.CurveHeight;

	JumpAttack_Bezier.A = GetActorLocation();
	JumpAttack_Bezier.B = Point;
	JumpAttack_Bezier.C = PlayerCharacter->GetActorLocation() + GetDirectionToPlayer() * -JumpAttack_Bezier.EndPointOffsetDistance + FVector(0.0f, 0.0f, 50.0f);

	if (JumpAttack_Bezier.bDebug)
	{
		DrawDebugLine(GetWorld(), JumpAttack_Bezier.A, JumpAttack_Bezier.B, FColor::Green, true, 5.0f, 0, 5.0f);
		DrawDebugLine(GetWorld(), JumpAttack_Bezier.B, JumpAttack_Bezier.C, FColor::Green, true, 5.0f, 0, 5.0f);
	}

	BossAIController->StopMovement();
	JumpAttackTimelineComponent->PlayFromStart();
}

void AMordath::DoJumpAttack()
{
	const float Time = JumpAttack_Bezier.Curve->GetFloatValue(JumpAttackTimelineComponent->GetPlaybackPosition());
	
	// Create points on the curve
	const FVector D = FMath::Lerp(JumpAttack_Bezier.A, JumpAttack_Bezier.B, Time);
	const FVector E = FMath::Lerp(JumpAttack_Bezier.B, JumpAttack_Bezier.C, Time);
	
	const FVector PointOnCurve = FMath::Lerp(D, E, Time);
	
	SetActorLocation(PointOnCurve);
	
	if (JumpAttack_Bezier.bDebug)
		DrawDebugPoint(GetWorld(), PointOnCurve, 10.0f, FColor::White, true, 5.0f);
}

void AMordath::BeginJumpAttackWithDash()
{
	BossStateMachine->PushState("DashToJump");

	// Create the main points of the bezier curve
	FVector Point = GetActorLocation() + GetActorRightVector() * (CombatSettings.DashDistance / 2.0f);
	Point.Z = Dash_Bezier.CurveHeight;

	Dash_Bezier.A = GetActorLocation();
	Dash_Bezier.B = Point;
	Dash_Bezier.C = Point + GetActorForwardVector() * (CombatSettings.DashDistance / 2.0f);

	if (Dash_Bezier.bDebug)
	{
		DrawDebugLine(GetWorld(), Dash_Bezier.A, Dash_Bezier.B, FColor::Green, true, 5.0f, 0, 5.0f);
		DrawDebugLine(GetWorld(), Dash_Bezier.B, Dash_Bezier.C, FColor::Green, true, 5.0f, 0, 5.0f);
	}

	BossAIController->StopMovement();
	DashTimelineComponent->PlayFromStart();
}

void AMordath::DoDash()
{
	const float Time = Dash_Bezier.Curve->GetFloatValue(DashTimelineComponent->GetPlaybackPosition());

	// Create points on the curve
	const FVector D = FMath::Lerp(Dash_Bezier.A, Dash_Bezier.B, Time);
	const FVector E = FMath::Lerp(Dash_Bezier.B, Dash_Bezier.C, Time);

	const FVector PointOnCurve = FMath::Lerp(D, E, Time);

	SetActorLocation(PointOnCurve);

	if (Dash_Bezier.bDebug)
		DrawDebugPoint(GetWorld(), PointOnCurve, 10.0f, FColor::White, true, 5.0f);
}

float AMordath::GetDistanceToPlayer() const
{
	const float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());
	//ULog::DebugMessage(INFO, FString("Distance: ") + FString::SanitizeFloat(Distance), true);
	return Distance;
}

FVector AMordath::GetDirectionToPlayer() const
{
	FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	//ULog::DebugMessage(INFO, FString("Direction: ") + Direction.ToString(), true);
	return Direction;
}
