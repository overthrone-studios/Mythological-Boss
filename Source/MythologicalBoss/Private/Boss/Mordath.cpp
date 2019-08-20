// Copyright Overthrone Studios 2019

#include "Mordath.h"
#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneHUD.h"
#include "BossAIController.h"
#include "Boss/MordathAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "HUD/MasterHUD.h"
#include "HUD/FSMVisualizerHUD.h"
#include "TimerManager.h"
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
	FSM = CreateDefaultSubobject<UFSM>(FName("Boss FSM"));
	FSM->AddState(0, "Idle");
	FSM->AddState(1, "Follow");
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


	// Bind state events to our functions
	FSM->GetState(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterIdleState);
	FSM->GetState(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateIdleState);
	FSM->GetState(0)->OnExitState.AddDynamic(this, &AMordath::OnExitIdleState);

	FSM->GetState(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFollowState);
	FSM->GetState(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFollowState);
	FSM->GetState(1)->OnExitState.AddDynamic(this, &AMordath::OnExitFollowState);

	FSM->GetState(3)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack1State);
	FSM->GetState(3)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack1State);
	FSM->GetState(3)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack1State);

	FSM->GetState(4)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack2State);
	FSM->GetState(4)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack2State);
	FSM->GetState(4)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack2State);

	FSM->GetState(5)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack3State);
	FSM->GetState(5)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack3State);
	FSM->GetState(5)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack3State);

	FSM->GetState(6)->OnEnterState.AddDynamic(this, &AMordath::OnEnterHeavyAttack1State);
	FSM->GetState(6)->OnUpdateState.AddDynamic(this, &AMordath::UpdateHeavyAttack1State);
	FSM->GetState(6)->OnExitState.AddDynamic(this, &AMordath::OnExitHeavyAttack1State);

	FSM->GetState(7)->OnEnterState.AddDynamic(this, &AMordath::OnEnterHeavyAttack2State);
	FSM->GetState(7)->OnUpdateState.AddDynamic(this, &AMordath::UpdateHeavyAttack2State);
	FSM->GetState(7)->OnExitState.AddDynamic(this, &AMordath::OnExitHeavyAttack2State);

	FSM->GetState(8)->OnEnterState.AddDynamic(this, &AMordath::OnEnterHeavyAttack3State);
	FSM->GetState(8)->OnUpdateState.AddDynamic(this, &AMordath::UpdateHeavyAttack3State);
	FSM->GetState(8)->OnExitState.AddDynamic(this, &AMordath::OnExitHeavyAttack3State);

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

	FSM->InitState(1);

	// Create a range FSM
	RangeFSM = CreateDefaultSubobject<UFSM>(FName("Range FSM"));
	RangeFSM->AddState(0, "Close");
	RangeFSM->AddState(1, "Mid");
	RangeFSM->AddState(2, "Far");

	RangeFSM->GetState(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterCloseRange);
	RangeFSM->GetState(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateCloseRange);
	RangeFSM->GetState(0)->OnExitState.AddDynamic(this, &AMordath::OnExitCloseRange);

	RangeFSM->GetState(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterMidRange);
	RangeFSM->GetState(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateMidRange);
	RangeFSM->GetState(1)->OnExitState.AddDynamic(this, &AMordath::OnExitMidRange);

	RangeFSM->GetState(2)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFarRange);
	RangeFSM->GetState(2)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFarRange);
	RangeFSM->GetState(2)->OnExitState.AddDynamic(this, &AMordath::OnExitFarRange);

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
	GetCharacterMovement()->MaxWalkSpeed = MovementSettings.WalkSpeed;

	// Configure character settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossAIController::StaticClass();

	// Timeline components
	JumpAttackTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(FName("Jump Attack Timeline"));
	DashTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(FName("Dash Timeline"));

	// Configure bezier curves
	Combat.AttackSettings.JumpAttack_Bezier.PlaybackSpeed = 2.0f;
	Combat.AttackSettings.JumpAttack_Bezier.CurveHeight = 1000.0f;
	Combat.AttackSettings.JumpAttack_Bezier.EndPointOffsetDistance = 100.0f;

	Combat.DashSettings.Dash_Bezier.PlaybackSpeed = 2.0f;
	Combat.DashSettings.Dash_Bezier.CurveHeight = 190.0f;
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	InitTimelineComponent(JumpAttackTimelineComponent, JumpAttack_Bezier.Curve, JumpAttack_Bezier.PlaybackSpeed, "DoJumpAttack", "FinishJumpAttack");
	InitTimelineComponent(DashTimelineComponent, Dash_Bezier.Curve, Dash_Bezier.PlaybackSpeed, "DoDash", "OnDashFinished");

	MovementComponent->MaxWalkSpeed = GetWalkSpeed();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	MordathAnimInstance = Cast<UMordathAnimInstance>(GetMesh()->GetAnimInstance());
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("BossFSMVisualizer"));

	// Initialize game instance variables
	GameInstance->BossInfo.StartingHealth = HealthComponent->GetDefaultHealth();
	GameInstance->BossInfo.Health = HealthComponent->GetCurrentHealth();
	GameInstance->BossInfo.OnLowHealth.AddDynamic(this, &AMordath::OnLowHealth);
	GameInstance->OnPlayerDeath.AddDynamic(this, &AMordath::OnPlayerDeath);
	GameInstance->OnSecondStage.AddDynamic(this, &AMordath::OnSecondStageHealth);
	GameInstance->OnThirdStage.AddDynamic(this, &AMordath::OnThirdStageHealth);
	GameInstance->Boss = this;
	SendInfo();

	ChooseCombo();

	GetWorld()->GetTimerManager().SetTimer(UpdateInfoTimerHandle, this, &AMordath::SendInfo, 0.05f, true);

	ensureAlways(ComboSettings.FirstStageCombos.Num() != 0);

	// Begin the state machines
	FSM->Start();
	RangeFSM->Start();
	StageFSM->Start();
}

void AMordath::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameInstance->PlayerInfo.bParrySucceeded && FSM->GetActiveStateID() != 14 /*Stunned*/)
	{
		FSM->PopState();
		FSM->PushState("Stunned");

		// Shake the camera
		PlayerController->ClientPlayCameraShake(CameraShakes.Stun.Shake, CameraShakes.Stun.Intensity);
	}
}

void AMordath::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
}

// Main FSM
#pragma region Idle
void AMordath::OnEnterIdleState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());
}

void AMordath::UpdateIdleState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (GameInstance->PlayerInfo.bIsDead)
		return;

	FacePlayer();

	if (ChosenCombo)
		FSM->PushState("Follow");
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

	if (!ChosenCombo)
	{
		ULog::DebugMessage(ERROR, FString("There are no combos in the list. A crash will occur!"), true);
		return;
	}

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
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// Check for destructible objects and destroy them
	if (ShouldDestroyDestructibleObjects())
	{
		BossAIController->StopMovement();
		FSM->PushState(3);
		return;
	}

	// Move towards the player
	if (GetDistanceToPlayer() > AcceptanceRadius - AcceptanceRadius/2.0f && !IsInvincible())
		AddMovementInput(GetDirectionToPlayer());

	FacePlayer();

	// Decide which attack to choose
	switch (RangeFSM->GetActiveStateID())
	{
	case 0 /*Close*/:
		if (!GetWorldTimerManager().IsTimerActive(ComboDelayTimerHandle) && 
			!GetWorldTimerManager().IsTimerActive(ChosenCombo->GetDelayTimer()))
			ChooseAttack();
	break;

	case 1 /*Mid*/:
		//BeginDash(Dash_Forward);
	break;

	case 2 /*Far*/:
		if (!GetWorldTimerManager().IsTimerActive(JumpAttackCooldownTimerHandle))
			BeginJumpAttack();
	break;

	default:
	break;
	}
}

void AMordath::OnExitFollowState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
}
#pragma endregion 

#pragma region Light Attack 1
void AMordath::OnEnterLightAttack1State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptLightAttack = true;
}

void AMordath::UpdateLightAttack1State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.4f)
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitLightAttack1State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptLightAttack = false;
}
#pragma endregion

#pragma region Light Attack 2
void AMordath::OnEnterLightAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondLightAttack = true;
}

void AMordath::UpdateLightAttack2State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.7f)
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitLightAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondLightAttack = false;
}
#pragma endregion

#pragma region Light Attack 3
void AMordath::OnEnterLightAttack3State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptThirdLightAttack = true;
}

void AMordath::UpdateLightAttack3State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.2f)
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitLightAttack3State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptThirdLightAttack = false;
}
#pragma endregion 

#pragma region Heavy Attack 1
void AMordath::OnEnterHeavyAttack1State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptHeavyAttack = true;
}

void AMordath::UpdateHeavyAttack1State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.2f)
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitHeavyAttack1State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptHeavyAttack = false;
}
#pragma endregion

#pragma region Heavy Attack 2
void AMordath::OnEnterHeavyAttack2State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondHeavyAttack = true;
}

void AMordath::UpdateHeavyAttack2State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.4f)
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitHeavyAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondHeavyAttack = false;

	GetWorldTimerManager().SetTimer(JumpAttackCooldownTimerHandle, this, &AMordath::FinishCooldown, Combat.AttackSettings.JumpAttackCooldown);
}
#pragma endregion

#pragma region Heavy Attack 3
void AMordath::OnEnterHeavyAttack3State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptThirdHeavyAttack = true;
}

void AMordath::UpdateHeavyAttack3State()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayer();

	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.3f)
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitHeavyAttack3State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptThirdHeavyAttack = false;
}
#pragma endregion

#pragma region Damaged
void AMordath::OnEnterDamagedState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	bIsHit = true;
	AnimInstance->bIsHit = true;
}

void AMordath::UpdateDamagedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	// If damaged animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.4f)
		FSM->PopState();
}

void AMordath::OnExitDamagedState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	bIsHit = false;
	AnimInstance->bIsHit = false;

	if (ChosenCombo)
		NextAttack();
}
#pragma endregion

#pragma region Death
void AMordath::OnEnterDeathState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsDead = true;

	GameInstance->OnBossDeath.Broadcast();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(DeathExpiryTimerHandle, this, &AMordath::DestroySelf, DeathTime);
}

void AMordath::UpdateDeathState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

}

void AMordath::OnExitDeathState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bIsDead = false;
}
#pragma endregion

#pragma region Stunned
void AMordath::OnEnterStunnedState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	// Reset hit count
	HitCounter = 0;

	BossAIController->StopMovement();

	MordathAnimInstance->bIsStunned = true;

	GetWorldTimerManager().SetTimer(StunExpiryTimerHandle, this, &AMordath::FinishStun, Combat.StunSettings.Duration);
}

void AMordath::UpdateStunnedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

}

void AMordath::OnExitStunnedState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	if (ChosenCombo)
		NextAttack();

	GameInstance->PlayerInfo.bParrySucceeded = false;
	MordathAnimInstance->bIsStunned = false;
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
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

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

	// Reset hit count
	HitCounter = 0;
}

void AMordath::UpdateDashState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayer();
}

void AMordath::OnExitDashState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &AMordath::FinishCooldown, Combat.DashSettings.DashCooldown);
}
#pragma endregion

#pragma region Beaten
void AMordath::OnEnterBeatenState()
{
	MordathAnimInstance->bIsBeaten = true;

	LaunchCharacter(-GetActorForwardVector() * 1000, true, true);
}

void AMordath::UpdateBeatenState()
{
}

void AMordath::OnExitBeatenState()
{
	MordathAnimInstance->bIsBeaten = false;
}
#pragma endregion

// Range FSM
#pragma region Close Range
void AMordath::OnEnterCloseRange()
{
	FSMVisualizer->HighlightState(RangeFSM->GetActiveStateName().ToString());

	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

	if (GetDistanceToPlayer() > AcceptanceRadius + 200.0f)
		RangeFSM->PushState("Mid");
}

void AMordath::UpdateCloseRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());

	if (GetDistanceToPlayer() > AcceptanceRadius + 200.0f)
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

	MovementComponent->MaxWalkSpeed = MovementSettings.MidRangeWalkSpeed;

	if (GetDistanceToPlayer() <= AcceptanceRadius)
	{
		RangeFSM->PushState("Close");
		return;
	}

	if (GetDistanceToPlayer() > MidRangeRadius)
		RangeFSM->PushState("Far");
}

void AMordath::UpdateMidRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());

	if (GetDistanceToPlayer() <= AcceptanceRadius)
		RangeFSM->PushState("Close");

	if (GetDistanceToPlayer() > MidRangeRadius)
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

	if (GetDistanceToPlayer() < MidRangeRadius)
		RangeFSM->PushState("Mid");
}

void AMordath::UpdateFarRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());

	if (GetDistanceToPlayer() < MidRangeRadius)
		RangeFSM->PushState("Mid");
}

void AMordath::OnExitFarRange()
{
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());
}
#pragma endregion 

// Stage FSM
#pragma region Stage 1
void AMordath::OnEnterFirstStage()
{
	FSMVisualizer->HighlightState(StageFSM->GetActiveStateName().ToString());
}

void AMordath::UpdateFirstStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	// Can we enter the second stage?
	if (HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * SecondStageHealth && 
		HealthComponent->GetCurrentHealth() > HealthComponent->GetDefaultHealth() * ThirdStageHealth)
		GameInstance->OnSecondStage.Broadcast();
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
}

void AMordath::UpdateSecondStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	// Can we enter the third stage?
	if (HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * ThirdStageHealth && 
		HealthComponent->GetCurrentHealth() > 0.0f)
		GameInstance->OnThirdStage.Broadcast();
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
}

void AMordath::UpdateThirdStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());
}

void AMordath::OnExitThirdStage()
{
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());
}
#pragma endregion 

void AMordath::OnPlayerDeath()
{
	BossAIController->StopMovement();
	JumpAttackTimelineComponent->Stop();
	DashTimelineComponent->Stop();

	FSM->RemoveAllStatesFromStack();
	FSM->PushState("Laugh");
}

void AMordath::OnDashFinished()
{
	FSM->PopState();

	// Resume movement
	MovementComponent->SetMovementMode(MOVE_Walking);

	// Are we still waiting to initiate the next attack?
	if (GetWorldTimerManager().IsTimerActive(ChosenCombo->GetDelayTimer()) || FSM->GetActiveStateName() == "Death")
		return;

	if (GetDistanceToPlayer() > AcceptanceRadius)
	{
		BeginJumpAttack();
		return;
	}

	switch (ChosenCombo->GetCurrentAttackInfo()->Attack)
	{
		case LightAttack_1:
			FSM->PushState("Light Attack 1");
		break;

		case LightAttack_2:
			FSM->PushState("Light Attack 2");
		break;

		case LightAttack_3:
			FSM->PushState("Light Attack 3");
		break;

		case HeavyAttack_1:
			FSM->PushState("Heavy Attack 1");
		break;

		case HeavyAttack_2:
			BeginJumpAttack();
		break;

		case HeavyAttack_3:
			FSM->PushState("Heavy Attack 3");
		break;

		default:
		break;
	}
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

void AMordath::DestroySelf()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());

	Destroy();
}

void AMordath::FinishCooldown()
{
	//ULog::DebugMessage(SUCCESS, "Cooldown finished", true);
}

void AMordath::UpdateCharacterInfo()
{
	GameInstance->BossInfo.Health = HealthComponent->GetSmoothedHealth();
	GameInstance->BossInfo.Location = GetActorLocation();
}

void AMordath::BroadcastLowHealth()
{
	GameInstance->BossInfo.OnLowHealth.Broadcast();
	bWasLowHealthEventTriggered = true;
}

void AMordath::OnLowHealth()
{
	ChangeHitboxSize(Combat.AttackSettings.AttackRadiusOnLowHealth);
}

void AMordath::FinishStun()
{
	FSM->PopState();
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
		// Is the combo data asset valid at 'Index'
		if (CachedCombos[ComboIndex])
		{
			ChosenCombo = CachedCombos[ComboIndex];

			if (Debug.bLogCurrentCombo)
				ULog::DebugMessage(SUCCESS, "Combo " + ChosenCombo->GetName() + " chosen", true);

			ChosenCombo->Init();

			CachedCombos.Remove(ChosenCombo);
		}
		else
		{
			ULog::DebugMessage(WARNING, FString("Combo asset at index ") + FString::FromInt(ComboIndex) + FString(" is not valid"), true);
		}

		MovementComponent->MaxWalkSpeed = GetWalkSpeed();
	}
	else
	{
		ComboIndex = 0;

		switch (StageFSM->GetActiveStateID())
		{
		case 0:
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 1 combos", true);

			CachedCombos = ComboSettings.FirstStageCombos;
		break;

		case 1:
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 2 combos", true);

			CachedCombos = ComboSettings.SecondStageCombos;
		break;

		case 2:
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 3 combos", true);

			CachedCombos = ComboSettings.ThirdStageCombos;
		break;

		default:
		break;
		}

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

	if (Debug.bLogComboDelayTime)
		ULog::DebugMessage(INFO, "Delaying: " + FString::SanitizeFloat(NewDelayTime) + " before next combo", true);

	MovementComponent->MaxWalkSpeed = MovementComponent->MaxWalkSpeed/2.0f;
}

void AMordath::ChooseAttack()
{
	if (FSM->GetActiveStateName() == "Dash")
		FSM->PopState();

	// Disable movement when we are going to attack
	MovementComponent->SetMovementMode(MOVE_None);

	switch (ChosenCombo->GetCurrentAttackInfo()->Attack)
	{
		case LightAttack_1:
		if (ChosenCombo->GetCurrentAttackInfo()->bCanDashWithAttack)
			BeginDash(ChosenCombo->GetCurrentAttackInfo()->DashType);
		else
			FSM->PushState("Light Attack 1");
		break;

		case LightAttack_2:
		if (ChosenCombo->GetCurrentAttackInfo()->bCanDashWithAttack)
			BeginDash(ChosenCombo->GetCurrentAttackInfo()->DashType);
		else
			FSM->PushState("Light Attack 2");
		break;

		case LightAttack_3:
		if (ChosenCombo->GetCurrentAttackInfo()->bCanDashWithAttack && GetDistanceToPlayer() > AcceptanceRadius - AcceptanceRadius/2.0f)
			BeginDash(ChosenCombo->GetCurrentAttackInfo()->DashType);
		else
			FSM->PushState("Light Attack 3");
		break;

		case HeavyAttack_1:
		if (ChosenCombo->GetCurrentAttackInfo()->bCanDashWithAttack)
			BeginDash(ChosenCombo->GetCurrentAttackInfo()->DashType);
		else
			FSM->PushState("Heavy Attack 1");
		break;

		case HeavyAttack_2:
		if (ChosenCombo->GetCurrentAttackInfo()->bCanDashWithAttack)
			BeginDash(ChosenCombo->GetCurrentAttackInfo()->DashType);
		else
			BeginJumpAttack();
		break;

		case HeavyAttack_3:
		if (ChosenCombo->GetCurrentAttackInfo()->bCanDashWithAttack)
			BeginDash(ChosenCombo->GetCurrentAttackInfo()->DashType);
		else
			FSM->PushState("Heavy Attack 3");
		break;

		default:
		break;
	}
}

void AMordath::NextAttack()
{
	if (ChosenCombo->IsDelayEnabled() && !GetWorldTimerManager().IsTimerActive(ChosenCombo->GetDelayTimer()))
	{
		const float Min = FMath::Clamp(ChosenCombo->GetDelayTime() - ChosenCombo->GetDeviation(), 0.0f, 100.0f);
		const float Max = FMath::Clamp(ChosenCombo->GetDelayTime() + ChosenCombo->GetDeviation(), 0.0f, 100.0f + ChosenCombo->GetDeviation());
		const float NewDelay = FMath::RandRange(Min, Max);

		if (NewDelay > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(ChosenCombo->GetDelayTimer(), this, &AMordath::NextAttack, NewDelay);
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

void AMordath::EnableInvincibility()
{
	bCanBeDamaged = false;
}

void AMordath::DisableInvincibility()
{
	// Evade
	const EDashType_Combo DashType = static_cast<EDashType_Combo>(FMath::RandRange(1, 3));
	BeginDash(DashType);

	bCanBeDamaged = true;
}

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead
	if (FSM->GetActiveStateName() == "Death")
		return DamageAmount;

	// Shake the camera
	PlayerController->ClientPlayCameraShake(CameraShakes.Damaged.Shake, CameraShakes.Damaged.Intensity);

	// Apply damage once
	if (!bIsHit && HitCounter < Combat.MaxHitsBeforeInvincibility && !GetWorldTimerManager().IsTimerActive(InvincibilityTimerHandle))
	{
		HitCounter++;

		if (Debug.bLogHits)
			ULog::DebugMessage(INFO, "Hit Count: " + FString::FromInt(HitCounter), true);

		if (FSM->GetActiveStateName() != "Stunned")
		{
			// Cancel current animation and enter the damaged state
			FSM->PopState();
			FSM->PushState("Damaged");
		}

		// Apply hit stop
		PauseAnimsWithTimer();

		// Lose health
		if (HealthComponent->IsUsingSmoothBar())
			StartLosingHealth(DamageAmount);
		else
			DecreaseHealth(DamageAmount);
	}

	// When we have reached the maximum amount of hits we can tolerate, enable invincibility
	if (HitCounter == Combat.MaxHitsBeforeInvincibility && !GetWorldTimerManager().IsTimerActive(InvincibilityTimerHandle))
	{
		// Reset our hits
		HitCounter = 0;

		// Become invincible and set a timer to disable invinciblity after 'X' seconds
		EnableInvincibility();
		GetWorldTimerManager().SetTimer(InvincibilityTimerHandle, this, &AMordath::DisableInvincibility, Combat.InvincibilityTimeAfterDamage);

		// Cancel our current animation and enter the downed state
		FSM->PopState();
		FSM->PushState("Beaten");

		// Lose health
		if (HealthComponent->IsUsingSmoothBar())
			StartLosingHealth(DamageAmount);
		else
			DecreaseHealth(DamageAmount);
	}

	// Are we dead?
	if (HealthComponent->GetCurrentHealth() <= 0.0f && FSM->GetActiveStateName() != "Death")
	{
		Die();
	}

	return DamageAmount;
}

void AMordath::ChangeHitboxSize(const float NewRadius)
{
	Combat.AttackSettings.AttackRadius = NewRadius;
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
	GameInstance->BossInfo.Health = HealthComponent->GetSmoothedHealth();
	GameInstance->BossInfo.Location = GetActorLocation();
}

void AMordath::BeginJumpAttack()
{
	if (JumpAttackTimelineComponent->IsPlaying())
		return;
	
	FSM->PopState("Dash");
	FSM->PushState("Heavy Attack 2");

	// Create the main points of the bezier curve
	FVector Point = PlayerCharacter->GetActorLocation() + GetDirectionToPlayer() * -(GetDistanceToPlayer() / 2.0f);
	Point.Z = JumpAttack_Bezier.CurveHeight;

	JumpAttack_Bezier.A = GetActorLocation();
	JumpAttack_Bezier.B = Point;
	JumpAttack_Bezier.C = PlayerCharacter->GetActorLocation() + GetDirectionToPlayer() * -JumpAttack_Bezier.EndPointOffsetDistance + FVector(0.0f, 0.0f, 90.0f);

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

void AMordath::FinishJumpAttack()
{
}

bool AMordath::IsStunned()
{
	return FSM->GetActiveStateID() == 14;
}

void AMordath::BeginDash(const enum EDashType_Combo DashType)
{
	// Are we already dashing?
	if (DashTimelineComponent->IsPlaying() || FSM->GetActiveStateName() == "Death")
		return;

	// Enter the dash state
	FSM->PopState();
	FSM->PushState("Dash");

	Dash_Bezier.A = GetActorLocation();

	const float DistanceToPlayer = FVector::Distance(GetActorLocation(), PlayerCharacter->GetActorLocation());

	// Handle each dash type and calculate the main bezier curve points
	switch (DashType)
	{
	case Dash_Forward:
		Dash_Bezier.B = GetActorLocation() + GetActorForwardVector() * (DistanceToPlayer/2.0f);
		Dash_Bezier.C = GetActorLocation() + GetActorForwardVector() * (DistanceToPlayer - ChosenCombo->GetCurrentAttackInfo()->AcceptanceRadius);
		Dash_Bezier.C.Z = GetActorLocation().Z;
	break;

	case Dash_Backward:
		Dash_Bezier.B = GetActorLocation() + -GetActorForwardVector() * (Combat.DashSettings.DashDistance/2.0f);
		Dash_Bezier.C = GetActorLocation() + -GetActorForwardVector() * Combat.DashSettings.DashDistance;
		Dash_Bezier.C.Z = GetActorLocation().Z;
	break;

	case Dash_Left:
	{
		FVector MidPoint = GetActorLocation() + GetActorRightVector() * Combat.DashSettings.DashDistance;
		MidPoint.Z = Dash_Bezier.CurveHeight;

		Dash_Bezier.B = MidPoint;
		Dash_Bezier.C = MidPoint + GetActorForwardVector() * Combat.DashSettings.DashDistance;
	}
	break;

	case Dash_Right:
	{
		FVector MidPoint = GetActorLocation() + -GetActorRightVector() * (Combat.DashSettings.DashDistance - ChosenCombo->GetCurrentAttackInfo()->AcceptanceRadius);
		MidPoint.Z = Dash_Bezier.CurveHeight;
		
		Dash_Bezier.B = MidPoint;
		Dash_Bezier.C = MidPoint + GetActorForwardVector() * (Combat.DashSettings.DashDistance - ChosenCombo->GetCurrentAttackInfo()->AcceptanceRadius);
	}
	break;
	}

	if (Dash_Bezier.bDebug)
	{
		DrawDebugLine(GetWorld(), Dash_Bezier.A, Dash_Bezier.B, FColor::Green, true, 5.0f, 0, 5.0f);
		DrawDebugLine(GetWorld(), Dash_Bezier.B, Dash_Bezier.C, FColor::Green, true, 5.0f, 0, 5.0f);
	}

	// Stop moving and do the dash
	BossAIController->StopMovement();
	DashTimelineComponent->SetPlayRate(IsInvincible() ? 1.0f : ChosenCombo->GetCurrentAttackInfo()->Speed);
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

	if (Debug.bLogDistance)
		ULog::DebugMessage(INFO, FString("Distance: ") + FString::SanitizeFloat(Distance), true);

	return Distance;
}

FVector AMordath::GetDirectionToPlayer() const
{
	FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
	Direction.Normalize();

	if (Debug.bLogDirection)
		ULog::DebugMessage(INFO, FString("Direction: ") + Direction.ToString(), true);

	return Direction;
}

void AMordath::Die()
{
	Super::Die();

	MordathAnimInstance->LeaveAllStates();

	JumpAttackTimelineComponent->Stop();
	DashTimelineComponent->Stop();

	BossAIController->StopMovement();
	MovementComponent->SetMovementMode(MOVE_None);
	MovementComponent->DisableMovement();

	FSM->RemoveAllStatesFromStack();
	FSM->PushState("Death");
}

void AMordath::PauseAnimsWithTimer()
{
	if (Combat.bEnableHitStop)
	{
		PauseAnims();
		GetWorldTimerManager().SetTimer(HitStopTimerHandle, this, &AMordath::UnPauseAnims, Combat.HitStopTime);
	}
}

bool AMordath::IsLightAttacking() const
{
	return FSM->GetActiveStateID() == 3 || FSM->GetActiveStateID() == 4 || FSM->GetActiveStateID() == 5;
}

bool AMordath::IsHeavyAttacking() const
{
	return FSM->GetActiveStateID() == 6 || FSM->GetActiveStateID() == 7 || FSM->GetActiveStateID() == 8;
}

bool AMordath::IsSpecialAttacking() const
{
	return FSM->GetActiveStateID() == 9 || FSM->GetActiveStateID() == 10 || FSM->GetActiveStateID() == 11;
}

float AMordath::GetWalkSpeed() const
{
	switch (RangeFSM->GetActiveStateID())
	{
	case 0 /*Close*/:
		return MovementSettings.WalkSpeed;

	case 1 /*Mid*/:
		return MovementSettings.MidRangeWalkSpeed;

	default:
		return MovementSettings.WalkSpeed;
	}
}
