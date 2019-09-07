// Copyright Overthrone Studios 2019

#include "Mordath.h"

#include "OverthroneFunctionLibrary.h"
#include "OverthroneGameInstance.h"
#include "OverthroneHUD.h"

#include "FSM.h"
#include "Log.h"

#include "Boss/BossAIController.h"
#include "Boss/MordathAnimInstance.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TeleportationComponent.h"

#include "Animation/AnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "HUD/MasterHUD.h"
#include "HUD/FSMVisualizerHUD.h"

#include "ConstructorHelpers.h"
#include "TimerManager.h"

AMordath::AMordath()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Mordath/Animations/AnimBP_Mordath.AnimBP_Mordath_C'"));

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

	// Teleportation component
	TeleportationComponent = CreateDefaultSubobject<UTeleportationComponent>(FName("Teleportation Component"));
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->MaxWalkSpeed = GetWalkSpeed();

	PlayerCharacter = UOverthroneFunctionLibrary::GetPlayerCharacter(this);
	MordathAnimInstance = Cast<UMordathAnimInstance>(GetMesh()->GetAnimInstance());
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("BossFSMVisualizer"));

	// Initialize game instance variables
	GameInstance->BossInfo.StartingHealth = HealthComponent->GetDefaultHealth();
	GameInstance->BossInfo.Health = HealthComponent->GetCurrentHealth();
	GameInstance->BossInfo.SmoothedHealth = HealthComponent->GetCurrentHealth();
	GameInstance->BossInfo.OnLowHealth.AddDynamic(this, &AMordath::OnLowHealth);
	GameInstance->OnPlayerDeath.AddDynamic(this, &AMordath::OnPlayerDeath);
	GameInstance->OnSecondStage.AddDynamic(this, &AMordath::OnSecondStageHealth);
	GameInstance->OnThirdStage.AddDynamic(this, &AMordath::OnThirdStageHealth);
	GameInstance->Boss = this;
	SendInfo();

	ChooseCombo();

	GetWorld()->GetTimerManager().SetTimer(UpdateInfoTimerHandle, this, &AMordath::SendInfo, 0.05f, true);

	// Begin the state machines
	FSM->Start();
	RangeFSM->Start();
	StageFSM->Start();
}

void AMordath::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameInstance->PlayerInfo.bIsDead)
	{
		AnimInstance->MovementSpeed = 0.0f;
		return;
	}
	
	AnimInstance->MovementSpeed = CurrentMovementSpeed;

	GameInstance->BossInfo.Location = GetActorLocation();

	if (GameInstance->PlayerInfo.bParrySucceeded && FSM->GetActiveStateID() != 14 /*Stunned*/)
	{
		FSM->PopState();
		FSM->PushState("Stunned");

		// Shake the camera
		PlayerController->ClientPlayCameraShake(CameraShakes.Stun.Shake, CameraShakes.Stun.Intensity);
	}

#if !UE_BUILD_SHIPPING
	if (Debug.bLogMovementSpeed)
		ULog::Number(MovementComponent->MaxWalkSpeed, "Movement Speed: ", true);
#endif
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

	if (GameInstance->PlayerInfo.bIsDead)
		return;

	FacePlayer();

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
	MovementComponent->MaxWalkSpeed = GetWalkSpeed();

	if (!ChosenCombo)
	{
		#if !UE_BUILD_SHIPPING
		ULog::DebugMessage(ERROR,FString("There are no combos in the list. A crash will occur!"),true);
		#endif

		return;
	}

	if (ChosenCombo->IsAtLastAttack() && !IsWaitingForNewCombo())
	{
		if (ComboSettings.bDelayBetweenCombo)
			ChooseComboWithDelay();
		else
			ChooseCombo();
	}

	ChooseMovementDirection();
}

void AMordath::UpdateFollowState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	FacePlayer();

	if (IsWaitingForNewCombo() && GetDistanceToPlayer() < AcceptanceRadius)
	{
		FSM->PushState("Retreat");
		return;
	}

	if (IsWaitingForNewCombo() && GetDistanceToPlayer() < MidRangeRadius)
	{
		FSM->PushState("Thinking");
		return;
	}

	// Move towards the player
	if (GetDistanceToPlayer() > AcceptanceRadius)
	{
		if (!IsDelayingAttack())
			AddMovementInput(GetDirectionToPlayer());
		else
			FSM->PushState("Thinking");
	}
	else
		MovementComponent->MaxWalkSpeed = 0.0f;
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

	MovementComponent->MaxWalkSpeed = MovementSettings.MidRangeWalkSpeed / 2.0f;

	RetreatStateData.CalculateRetreatTime();

#if !UE_BUILD_SHIPPING
	if (Debug.bLogRetreatTime)
		ULog::Number(RetreatStateData.RetreatTime, "Retreat Time: ", true);
#endif
}

void AMordath::UpdateRetreatState()
{
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	FacePlayer();

	if (GetDistanceToPlayer() > MidRangeRadius)
		FSM->PopState();

	if (IsWaitingForNewCombo() && GetDistanceToPlayer() < AcceptanceRadius || Uptime <= RetreatStateData.RetreatTime)
		AddMovementInput(-GetDirectionToPlayer());
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

	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

	ChooseMovementDirection();

	ThinkStateData.CalculateThinkTime();

	MordathAnimInstance->bIsThinking = true;

#if !UE_BUILD_SHIPPING
	if (Debug.bLogThinkTime)
		ULog::Number(ThinkStateData.ThinkTime, "Think Time: ", true);
#endif
}

void AMordath::UpdateThinkState()
{
	const float Uptime = FSM->GetActiveStateUptime();
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	FacePlayer();

	EncirclePlayer();

	if (!IsWaitingForNewCombo() && Uptime >= ThinkStateData.ThinkTime)
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
void AMordath::OnEnterLightAttack1State()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptLightAttack = true;
}

void AMordath::UpdateLightAttack1State()
{
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime < 0.3f)
		FacePlayer();

	// If attack animation has finished, go back to previous state
	if(AnimInstance->AnimTimeRemaining <= 0.1f)
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
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime < 0.3f)
		FacePlayer();

	// If attack animation has finished, go back to previous state
	if(AnimInstance->AnimTimeRemaining <= 0.1f)
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
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime < 0.3f)
		FacePlayer();

	// If attack animation has finished, go back to previous state
	if(AnimInstance->AnimTimeRemaining <= 0.1f)
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
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime < 0.3f)
		FacePlayer();

	// If attack animation has finished, go back to previous state
	if(AnimInstance->AnimTimeRemaining <= 0.1f)
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
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime < 0.3f)
		FacePlayer();

	// If attack animation has finished, go back to previous state
	if(AnimInstance->AnimTimeRemaining <= 0.1f)
	{
		NextAttack();

		FSM->PopState();
	}
}

void AMordath::OnExitHeavyAttack2State()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	AnimInstance->bAcceptSecondHeavyAttack = false;

	GetWorldTimerManager().SetTimer(JumpAttackCooldownTimerHandle,Combat.AttackSettings.JumpAttackCooldown,false);
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
	const float Uptime = FSM->GetActiveStateUptime();

	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	if (Uptime < 0.3f)
		FacePlayer();

	// If attack animation has finished, go back to previous state
	if(AnimInstance->AnimTimeRemaining <= 0.1f)
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

	AnimInstance->bIsHit = true;
}

void AMordath::UpdateDamagedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (AnimInstance->AnimTimeRemaining <= 0.1f)
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

	AnimInstance->bIsDead = true;

	GameInstance->OnBossDeath.Broadcast();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(DeathExpiryTimerHandle,this,&AMordath::DestroySelf,DeathTime);
}

void AMordath::UpdateDeathState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(),FSM->GetActiveStateUptime());

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

	MordathAnimInstance->bIsStunned = true;
}

void AMordath::UpdateStunnedState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AMordath::OnExitStunnedState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	GameInstance->PlayerInfo.bParrySucceeded = false;
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

	// Reset hit count
	HitCounter = 0;
}

void AMordath::UpdateDashState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(),FSM->GetActiveStateUptime());

	FacePlayer();
}

void AMordath::OnExitDashState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

}
#pragma endregion

#pragma region Beaten
void AMordath::OnEnterBeatenState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());

	MordathAnimInstance->bIsBeaten = true;
}

void AMordath::UpdateBeatenState()
{
	const float Uptime = FSM->GetActiveStateUptime();
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), Uptime);

	// If the recover time has finished, go back to previous state
	if (Uptime >= Combat.InvincibilityTimeAfterDamage)
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
}

void AMordath::UpdateTeleportState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(),FSM->GetActiveStateUptime());

	if(FSM->GetActiveStateUptime() > TeleportationComponent->GetTeleportTime())
	{
		if(ChosenCombo->GetCurrentAttackInfo()->bCanTeleportWithAttack)
			SetActorLocation(TeleportationComponent->FindLocationToTeleport(PlayerCharacter->GetActorLocation(),GameInstance->GetTeleportRadius(),GameInstance->PlayArea));

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
}

void AMordath::UpdateCloseRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());

	if (GetDistanceToPlayer() > AcceptanceRadius)
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
}

void AMordath::UpdateMidRange()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(),RangeFSM->GetActiveStateUptime());

	if (GetDistanceToPlayer() < AcceptanceRadius)
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

	if ((StageFSM->GetActiveStateID() == 1 /*Second Stage*/ || StageFSM->GetActiveStateID() == 2 /*Third Stage*/) &&
		ChosenCombo->GetCurrentAttackInfo()->bCanTeleportWithAttack)
	{
		FSM->PopState();
		FSM->PushState("Teleport");
	}
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
#pragma endregion

#pragma region Boss Stages
#pragma region Stage 1
void AMordath::OnEnterFirstStage()
{
	FSMVisualizer->HighlightState(StageFSM->GetActiveStateName().ToString());

	MordathAnimInstance->CurrentStage = Stage_1;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[0];
}

void AMordath::UpdateFirstStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	// Can we enter the second stage?
	if(HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * SecondStageHealth &&
		HealthComponent->GetCurrentHealth() > HealthComponent->GetDefaultHealth() * ThirdStageHealth)
	{
		GameInstance->OnSecondStage.Broadcast();
		return;
	}

	if (RangeFSM->GetActiveStateID() == 0 /*Close range*/ && !IsRecovering())
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

	MordathAnimInstance->CurrentStage = Stage_2;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];
}

void AMordath::UpdateSecondStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	// Can we enter the third stage?
	if (HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * ThirdStageHealth &&
		HealthComponent->GetCurrentHealth() > 0.0f)
	{
		GameInstance->OnThirdStage.Broadcast();
		return;
	}

	if (RangeFSM->GetActiveStateID() == 0 /*Close range*/ && !IsRecovering())
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

	MordathAnimInstance->CurrentStage = Stage_3;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];
}

void AMordath::UpdateThirdStage()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());

	if (RangeFSM->GetActiveStateID() == 0 /*Close range*/)
	{
		// Decide which attack to choose
		if (!IsWaitingForNewCombo() && !IsDelayingAttack() && !IsRecovering())
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
	ChangeHitboxSize(Combat.AttackSettings.AttackRadiusOnLowHealth);
}

void AMordath::OnPlayerDeath()
{
	BossAIController->StopMovement();

	FSM->RemoveAllStatesFromStack();
	FSM->PushState("Laugh");

	FSM->Stop();
	RangeFSM->Stop();
	StageFSM->Stop();
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

void AMordath::UpdateCharacterInfo()
{
	GameInstance->BossInfo.Health = HealthComponent->GetCurrentHealth();
	GameInstance->BossInfo.SmoothedHealth = HealthComponent->GetSmoothedHealth();
}

void AMordath::BroadcastLowHealth()
{
	GameInstance->BossInfo.OnLowHealth.Broadcast();
	bWasLowHealthEventTriggered = true;
}

void AMordath::FinishStun()
{
	FSM->PopState("Stunned");
}

void AMordath::BeginTakeDamage(const float DamageAmount)
{
	Combat.RecentDamage = DamageAmount;

	// Shake the camera
	PlayerController->ClientPlayCameraShake(CameraShakes.Damaged.Shake, CameraShakes.Damaged.Intensity);
}

void AMordath::ApplyDamage(const float DamageAmount)
{
	HitCounter++;

#if !UE_BUILD_SHIPPING
	if (Debug.bLogHits)
		ULog::DebugMessage(INFO, "Hit Count: " + FString::FromInt(HitCounter), true);
#endif

	if (FSM->GetActiveStateName() != "Stunned" && !InInvincibleState())
	{
		// Cancel current animation and enter the damaged state
		FSM->PopState();
		FSM->PushState("Damaged");
	}

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
	if (ComboSettings.bChooseRandomCombo)
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

			CachedCombos.Remove(ChosenCombo);
		}
		else
		{
			#if !UE_BUILD_SHIPPING
			ULog::DebugMessage(WARNING, FString("Combo asset at index ") + FString::FromInt(ComboIndex) + FString(" is not valid"), true);
			#endif
		}

		MovementComponent->MaxWalkSpeed = GetWalkSpeed();
	}
	else
	{
		ComboIndex = 0;

		switch (StageFSM->GetActiveStateID())
		{
		case 0:
			#if !UE_BUILD_SHIPPING
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 1 combos", true);
			#endif

			CachedCombos = ComboSettings.FirstStageCombos;
		break;

		case 1:
			#if !UE_BUILD_SHIPPING
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 2 combos", true);
			#endif

			CachedCombos = ComboSettings.SecondStageCombos;
		break;

		case 2:
			#if !UE_BUILD_SHIPPING
			if (Debug.bLogCurrentStageCombo)
				ULog::Info("Using stage 3 combos", true);
			#endif

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

	#if !UE_BUILD_SHIPPING
	if (Debug.bLogComboDelayTime)
		ULog::DebugMessage(INFO, "Delaying: " + FString::SanitizeFloat(NewDelayTime) + " before next combo", true);
	#endif

	MovementComponent->MaxWalkSpeed = MovementComponent->MaxWalkSpeed/2.0f;
}

void AMordath::ChooseAttack()
{
	if (FSM->GetActiveStateName() == "Dash")
		FSM->PopState();

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
			FSM->PushState("Heavy Attack 2");
		break;

		case HeavyAttack_3:
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

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead
	if (FSM->GetActiveStateName() == "Death" || AnimInstance->bIsHit)
		return DamageAmount;

	BeginTakeDamage(DamageAmount);

	// Apply damage once
	if (!AnimInstance->bIsHit && HitCounter < Combat.MaxHitsBeforeInvincibility && !GetWorldTimerManager().IsTimerActive(InvincibilityTimerHandle))
	{
		ApplyDamage(DamageAmount);

		GetMesh()->SetWorldScale3D(FVector(1.35f));
		GetWorldTimerManager().SetTimer(HitReactionTimerHandle, this, &AMordath::ResetMeshScale, 0.1f);
	}

	// When we have reached the maximum amount of hits we can tolerate, enable invincibility
	if (HitCounter == Combat.MaxHitsBeforeInvincibility && !GetWorldTimerManager().IsTimerActive(InvincibilityTimerHandle))
	{
		// Reset our hits
		HitCounter = 0;

		// Become invincible and set a timer to disable invincibility after 'X' seconds
		EnableInvincibility();
		GetWorldTimerManager().SetTimer(InvincibilityTimerHandle, this, &AMordath::DisableInvincibility, Combat.InvincibilityTimeAfterDamage);

		// Cancel our current animation and enter the downed state
		FSM->PushState("Beaten");

		UpdateHealth(DamageAmount);

		// Handled in blueprints
		OnAfterTakeDamage();
	}

	EndTakeDamage();

	return DamageAmount;
}

void AMordath::ChangeHitboxSize(const float NewRadius)
{
	Combat.AttackSettings.AttackRadius = NewRadius;
}

void AMordath::FacePlayer()
{
	FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	
	SetActorRotation(FRotator(GetControlRotation().Pitch, Direction.Rotation().Yaw, GetControlRotation().Roll));
}

void AMordath::SendInfo()
{
	GameInstance->BossInfo.Health = HealthComponent->GetCurrentHealth();
	GameInstance->BossInfo.SmoothedHealth = HealthComponent->GetSmoothedHealth();
}

bool AMordath::IsStunned()
{
	return FSM->GetActiveStateID() == 14;
}

void AMordath::ChooseMovementDirection()
{
	MoveDirection = FMath::RandRange(0, 1);
}

void AMordath::EncirclePlayer()
{
	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

	if (PlayerCharacter->GetInputAxisValue("MoveRight") > 0.0f)
	{
		AddMovementInput(GetActorRightVector());
		MordathAnimInstance->MovementDirection = 1;
	}
	else if (PlayerCharacter->GetInputAxisValue("MoveRight") < 0.0f)
	{
		AddMovementInput(-GetActorRightVector());
		MordathAnimInstance->MovementDirection = -1;
	}
	else
	{
		if (WantsMoveRight())
		{
			AddMovementInput(GetActorRightVector());
			MordathAnimInstance->MovementDirection = 1;
		}
		else
		{
			AddMovementInput(-GetActorRightVector());
			MordathAnimInstance->MovementDirection = -1;
		}
	}
}

void AMordath::ResetMeshScale()
{
	GetMesh()->SetWorldScale3D(FVector(1.3f));
}

float AMordath::GetDistanceToPlayer() const
{
	const float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

	#if !UE_BUILD_SHIPPING
	if (Debug.bLogDistance)
		ULog::DebugMessage(INFO, FString("Distance: ") + FString::SanitizeFloat(Distance), true);
	#endif

	return Distance;
}

FVector AMordath::GetDirectionToPlayer() const
{
	FVector Direction = PlayerCharacter->GetActorLocation() - GetActorLocation();
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

bool AMordath::IsAttacking() const
{
	return IsLightAttacking() || IsHeavyAttacking() || IsSpecialAttacking();
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

bool AMordath::IsCloseRange() const
{
	return RangeFSM->GetActiveStateID() == 0;
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
	return GetWorldTimerManager().IsTimerActive(ComboDelayTimerHandle);
}

bool AMordath::IsDelayingAttack()
{
	return GetWorldTimerManager().IsTimerActive(ChosenCombo->GetAttackDelayTimer());
}

bool AMordath::WantsMoveRight() const
{
	return MoveDirection == 1;
}

bool AMordath::IsRecovering() const
{
	return FSM->GetActiveStateID() == 17;
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

void FThinkStateData::CalculateThinkTime()
{
	const float Min = ThinkTime - RandomDeviation;
	const float Max = ThinkTime + RandomDeviation;

	ThinkTime = FMath::Clamp(FMath::FRandRange(Min, Max), 0.0f, Max);
}

void FRetreatStateData::CalculateRetreatTime()
{
	const float Min = RetreatTime - RandomDeviation;
	const float Max = RetreatTime + RandomDeviation;

	RetreatTime = FMath::Clamp(FMath::FRandRange(Min, Max), 0.0f, Max);
}
