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
#include "Boss/MordathGhost.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TeleportationComponent.h"
#include "Components/DashComponent.h"
#include "Components/AttackIndicatorComponent.h"

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

static bool GWantsMoveRight = true;

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
	//FSM->AddState(3, "Short Attack 1");
	//FSM->AddState(4, "Short Attack 2");
	//FSM->AddState(5, "Short Attack 3");
	//FSM->AddState(6, "Long Attack 1");
	//FSM->AddState(7, "Long Attack 2");
	//FSM->AddState(8, "Long Attack 3");
	//FSM->AddState(9, "Special Attack 1");
	//FSM->AddState(10, "Special Attack 2");
	//FSM->AddState(11, "Special Attack 3");
	FSM->AddState(12, "Damaged");
	FSM->AddState(13, "Death");
	FSM->AddState(14, "Stunned");
	FSM->AddState(15, "Laugh");
	FSM->AddState(16, "Dash");
	FSM->AddState(17, "Dash Combat");
	FSM->AddState(18, "Teleport");
	FSM->AddState(19, "Retreat");
	FSM->AddState(20, "Kick");
	FSM->AddState(21, "Recover");
	FSM->AddState(22, "Strafe");
	FSM->AddState(23, "Tired");
	FSM->AddState(24, "Back Hand");
	FSM->AddState(25, "Action");

	FSM->OnEnterAnyState.AddDynamic(this, &AMordath::OnEnterAnyState);
	FSM->OnUpdateAnyState.AddDynamic(this, &AMordath::UpdateAnyState);
	FSM->OnExitAnyState.AddDynamic(this, &AMordath::OnExitAnyState);

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

	FSM->GetState(17)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDashCombatState);
	FSM->GetState(17)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDashCombatState);
	FSM->GetState(17)->OnExitState.AddDynamic(this, &AMordath::OnExitDashCombatState);

	FSM->GetState(18)->OnEnterState.AddDynamic(this, &AMordath::OnEnterTeleportState);
	FSM->GetState(18)->OnUpdateState.AddDynamic(this, &AMordath::UpdateTeleportState);
	FSM->GetState(18)->OnExitState.AddDynamic(this, &AMordath::OnExitTeleportState);

	FSM->GetState(19)->OnEnterState.AddDynamic(this, &AMordath::OnEnterRetreatState);
	FSM->GetState(19)->OnUpdateState.AddDynamic(this, &AMordath::UpdateRetreatState);
	FSM->GetState(19)->OnExitState.AddDynamic(this, &AMordath::OnExitRetreatState);

	FSM->GetState(20)->OnEnterState.AddDynamic(this, &AMordath::OnEnterKickState);
	FSM->GetState(20)->OnUpdateState.AddDynamic(this, &AMordath::UpdateKickState);
	FSM->GetState(20)->OnExitState.AddDynamic(this, &AMordath::OnExitKickState);

	FSM->GetState(21)->OnEnterState.AddDynamic(this, &AMordath::OnEnterRecoverState);
	FSM->GetState(21)->OnUpdateState.AddDynamic(this, &AMordath::UpdateRecoverState);
	FSM->GetState(21)->OnExitState.AddDynamic(this, &AMordath::OnExitRecoverState);

	FSM->GetState(22)->OnEnterState.AddDynamic(this, &AMordath::OnEnterStrafeState);
	FSM->GetState(22)->OnUpdateState.AddDynamic(this, &AMordath::UpdateStrafeState);
	FSM->GetState(22)->OnExitState.AddDynamic(this, &AMordath::OnExitStrafeState);

	FSM->GetState(23)->OnEnterState.AddDynamic(this, &AMordath::OnEnterTiredState);
	FSM->GetState(23)->OnUpdateState.AddDynamic(this, &AMordath::UpdateTiredState);
	FSM->GetState(23)->OnExitState.AddDynamic(this, &AMordath::OnExitTiredState);

	FSM->GetState(24)->OnEnterState.AddDynamic(this, &AMordath::OnEnterBackHandState);
	FSM->GetState(24)->OnUpdateState.AddDynamic(this, &AMordath::UpdateBackHandState);
	FSM->GetState(24)->OnExitState.AddDynamic(this, &AMordath::OnExitBackHandState);

	FSM->GetState(25)->OnEnterState.AddDynamic(this, &AMordath::OnEnterActionState);
	FSM->GetState(25)->OnUpdateState.AddDynamic(this, &AMordath::UpdateActionState);
	FSM->GetState(25)->OnExitState.AddDynamic(this, &AMordath::OnExitActionState);

	FSM->InitState(0);

	// Create a range FSM
	RangeFSM = CreateDefaultSubobject<UFSM>(FName("Range FSM"));
	RangeFSM->AddState(0, "Close");
	RangeFSM->AddState(1, "Mid");
	RangeFSM->AddState(2, "Far");
	RangeFSM->AddState(3, "Super Close");

	RangeFSM->OnEnterAnyState.AddDynamic(this, &AMordath::OnEnterAnyRangeState);
	RangeFSM->OnUpdateAnyState.AddDynamic(this, &AMordath::UpdateAnyRangeState);
	RangeFSM->OnExitAnyState.AddDynamic(this, &AMordath::OnExitAnyRangeState);

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

	StageFSM->OnEnterAnyState.AddDynamic(this, &AMordath::OnEnterAnyStageState);
	StageFSM->OnUpdateAnyState.AddDynamic(this, &AMordath::UpdateAnyStageState);
	StageFSM->OnExitAnyState.AddDynamic(this, &AMordath::OnExitAnyStageState);

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
	FlashIndicator = CreateDefaultSubobject<UAttackIndicatorComponent>(FName("Flash Indicator Mesh"));
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

	ChooseCombo();

	ResetActionDamage();

	SuperCloseRange_ActionData = CurrentStageData->Combat.SuperCloseRangeActionData;

#if !UE_BUILD_SHIPPING
	GetCapsuleComponent()->SetHiddenInGame(false);
#else
	GetCapsuleComponent()->SetHiddenInGame(true);
#endif
}

void AMordath::Tick(const float DeltaTime)
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
	
	GameState->BossData.Location = CurrentLocation;
	GameState->BossData.LockOnBoneLocation = SKMComponent->GetSocketLocation(LockOnBoneName);
	//GameState->BossData.LockOnLocation = GameState->BossData.CurrentAttackType == LongAttack_1 ? FMath::Lerp(GameState->BossData.LockOnLocation, GameState->BossData.LockOnBoneLocation, 10* DeltaTime) : FMath::Lerp(GameState->BossData.LockOnLocation, CurrentLocation, 5* DeltaTime);

	DistanceToPlayer = GetDistanceToPlayer();
	DirectionToPlayer = GetDirectionToPlayer();

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->ForwardInput = ForwardInput;
	AnimInstance->RightInput = RightInput;

#if !UE_BUILD_SHIPPING
	// Range FSM
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());
	FSMVisualizer->UpdateStateFrames(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateFrames());

	// Stage FSM
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());
	FSMVisualizer->UpdateStateFrames(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateFrames());

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
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 5, "Current Action: " + CurrentActionData->Action->GetCurrentActionAsString()/*UOverthroneEnums::MordathAttackTypeToString(GameState->BossData.CurrentActionType)*/);
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 4, "Current Counter: " + CurrentActionData->Action->GetCounterTypeAsString()/*UOverthroneEnums::MordathAttackCounterTypeToString(GameState->BossData.CurrentCounterType)*/);
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 3, "Lock-on Location Z: " + FString::SanitizeFloat(GameState->BossData.LockOnBoneLocation.Z));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 2, "Action Damage: " + FString::SanitizeFloat(ActionDamage));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 1, "Current Combo: " + ChosenCombo->GetName());
#endif
}

void AMordath::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
}

#pragma region Any States
void AMordath::OnEnterAnyState()
{
	FSMVisualizer->HighlightState(FSM->GetActiveStateName().ToString());
}

void AMordath::UpdateAnyState()
{
	FSMVisualizer->UpdateStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
	FSMVisualizer->UpdateStateFrames(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateFrames());

	if (GameState->IsPlayerDead() && HasFinishedAction())
	{
		FSM->Stop();
	}
}

void AMordath::OnExitAnyState()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());

	FSMVisualizer->UpdatePreviousStateUptime(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateUptime());
	FSMVisualizer->UpdatePreviousStateFrames(FSM->GetActiveStateName().ToString(), FSM->GetActiveStateFrames());
}

void AMordath::OnEnterAnyRangeState()
{
	FSMVisualizer->HighlightState(RangeFSM->GetActiveStateName().ToString());
}

void AMordath::UpdateAnyRangeState()
{
	FSMVisualizer->UpdateStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());
	FSMVisualizer->UpdateStateFrames(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateFrames());
}

void AMordath::OnExitAnyRangeState()
{
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());

	FSMVisualizer->UpdatePreviousStateUptime(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateUptime());
	FSMVisualizer->UpdatePreviousStateFrames(RangeFSM->GetActiveStateName().ToString(), RangeFSM->GetActiveStateFrames());
}

void AMordath::OnEnterAnyStageState()
{
	FSMVisualizer->HighlightState(StageFSM->GetActiveStateName().ToString());
}

void AMordath::UpdateAnyStageState()
{
	FSMVisualizer->UpdateStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());
	FSMVisualizer->UpdateStateFrames(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateFrames());
}

void AMordath::OnExitAnyStageState()
{
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());

	FSMVisualizer->UpdatePreviousStateUptime(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateUptime());
	FSMVisualizer->UpdatePreviousStateFrames(StageFSM->GetActiveStateName().ToString(), StageFSM->GetActiveStateFrames());
}
#pragma endregion

#pragma region Boss States
#pragma region Idle
void AMordath::OnEnterIdleState()
{
}

void AMordath::UpdateIdleState()
{
	if (GameState->PlayerData.bIsDead)
		return;

	FacePlayer();

	ForwardInput = 0.0f;
	RightInput = 0.0f;

	FSM->PushState("Thinking");
}

void AMordath::OnExitIdleState()
{

}
#pragma endregion

#pragma region Follow
void AMordath::OnEnterFollowState()
{
	if (!ChosenCombo)
	{
		#if !UE_BUILD_SHIPPING
		ULog::DebugMessage(ERROR,FString("There are no combos in the list. A crash will occur!"),true);
		#endif

		return;
	}

	if (ChosenCombo->IsAtLastAction() && !IsWaitingForNewCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboWithDelay();
		else
			ChooseCombo();
	}

	//const uint8 bWantsDashForward = FMath::RandRange(0, 1);
	//if (bWantsDashForward && !IsDashing() && DistanceToPlayer > CurrentStageData->GetDashDistanceThreshold() && !DashComponent->IsCooldownActive())
	//{
	//	if (IsMidRange())
	//		DashType = Dash_Forward;
	//	else if (IsCloseRange() || IsSuperCloseRange())
	//		DashType = Dash_Backward;
	//
	//	FSM->PushState("Dash");
	//	return;
	//}

	if (IsSuperCloseRange() && FVector::DotProduct(GetActorForwardVector(), DirectionToPlayer) < -0.3f && 
		(CurrentActionData->Action->ActionType != ATM_Dash_Forward && CurrentActionData->Action->ActionType != ATM_Dash_Backward &&
		CurrentActionData->Action->ActionType != ATM_Dash_Left && CurrentActionData->Action->ActionType != ATM_Dash_Right))
	{
		ULog::Yes("Can back hand?", true);
		FSM->PushState("Back Hand");
		return;
	}

	ChooseMovementDirection();
}

void AMordath::UpdateFollowState()
{
	if (IsTransitioning())
	{
		BossAIController->StopMovement();

		return;
	}

	FacePlayer();

	if (IsWaitingForNewCombo() && IsSuperCloseRange())
	{
		FSM->PushState("Retreat");
		return;
	}

	if (IsWaitingForNewCombo() && IsCloseRange())
	{
		FSM->PushState("Thinking");
		return;
	}

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
				StopMoving();
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
				StopMoving();
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
				StopMoving();
			}
		break;

		case BRM_Far:
			if (IsSuperCloseRange() || IsCloseRange() || IsMidRange())
			{
				MoveForward(-1);
			}
			else
			{
				StopMoving();
			}
		break;

		default:
			StopMoving();
		break;
		}
	}
	else
		FSM->PushState("Thinking");
}

void AMordath::OnExitFollowState()
{

}
#pragma endregion

#pragma region Retreat
void AMordath::OnEnterRetreatState()
{
	RetreatTime = CurrentStageData->RetreatStateData.CalculateRetreatTime();

#if !UE_BUILD_SHIPPING
	if (Debug.bLogRetreatTime)
		ULog::Number(RetreatTime, "Retreat Time: ", true);
#endif
}

void AMordath::UpdateRetreatState()
{
	const float Uptime = FSM->GetActiveStateUptime();

	if (IsTransitioning())
	{
		BossAIController->StopMovement();

		return;
	}

	FacePlayer();

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
}
#pragma endregion  

#pragma region Kick
void AMordath::OnEnterKickState()
{
	MordathAnimInstance->bCanKick = true;
}

void AMordath::UpdateKickState()
{
	if (CurrentActionData->Action->ActionType == ATM_Kick)
		FacePlayerBasedOnActionData(CurrentActionData->Action);
	else
		FacePlayer();

	if (AnimInstance->AnimTimeRemaining < 0.1f)
	{
		NextAction();

		FSM->PopState();
	}
}

void AMordath::OnExitKickState()
{
	MordathAnimInstance->bCanKick = false;
}
#pragma endregion  

#pragma region Recover
void AMordath::OnEnterRecoverState()
{
	MordathAnimInstance->bIsRecovering = true;
}

void AMordath::UpdateRecoverState()
{
	if (FSM->GetActiveStateUptime() > CurrentStageData->GetRecoverTime())
	{
		FSM->PopState();
	}
}

void AMordath::OnExitRecoverState()
{
	MordathAnimInstance->bIsRecovering = false;
}
#pragma endregion  

#pragma region Think
void AMordath::OnEnterThinkState()
{
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

	if (IsTransitioning())
	{
		BossAIController->StopMovement();

		return;
	}

	FacePlayer();

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
	MordathAnimInstance->bIsThinking = false;
}
#pragma endregion 

#pragma region Action
void AMordath::OnEnterActionState()
{
	PlayActionMontage();
}

void AMordath::UpdateActionState()
{
	StopMoving();

	if (CurrentActionData->Action->bConstantlyFacePlayer)
		FacePlayer();
	else
		FacePlayerBasedOnActionData(CurrentActionData->Action);
	
	// If action has finished, go back to previous state
	if (HasFinishedAction())
		FSM->PopState();
}

void AMordath::OnExitActionState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopActionMontage();

	NextAction();
}
#pragma endregion 

#pragma region Damaged
void AMordath::OnEnterDamagedState()
{
	GameState->BossData.bHasTakenDamage = true;
	AnimInstance->bIsHit = true;
}

void AMordath::UpdateDamagedState()
{
	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AMordath::OnExitDamagedState()
{
	AnimInstance->bIsHit = false;
	GameState->BossData.bHasTakenDamage = false;

	// Reset blend out time when we've been damaged
	if (PreviousActionMontage)
		PreviousActionMontage->BlendOut.SetBlendTime(0.5f);

	if (ChosenCombo)
		NextAction();
}
#pragma endregion

#pragma region Death
void AMordath::OnEnterDeathState()
{
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
	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->Stop();
}

void AMordath::OnExitDeathState()
{
	bIsDead = false;
	GameState->BossData.bIsDead = false;
	AnimInstance->bIsDead = false;
}
#pragma endregion

#pragma region Stunned
void AMordath::OnEnterStunnedState()
{
	// Reset hit count
	HitCounter = 0;

	StopActionMontage();

	GameState->BossData.bHasTakenDamage = true;
	MordathAnimInstance->bIsStunned = true;
}

void AMordath::UpdateStunnedState()
{
	if (AnimInstance->AnimTimeRemaining < 0.1f/* FSM->GetActiveStateUptime() > CurrentStageData->StunTime*/)
	{
		FSM->PopState();
		FSM->PushState("Recover");
	}
}

void AMordath::OnExitStunnedState()
{
	GameState->BossData.bHasTakenDamage = false;
	GameState->PlayerData.bParrySucceeded = false;

	MordathAnimInstance->bIsStunned = false;

	// Reset blend out time when we've been stunned
	if (PreviousActionMontage)
		PreviousActionMontage->BlendOut.SetBlendTime(0.5f);

	if (ChosenCombo)
		NextAction();
}
#pragma endregion

#pragma region Laugh
void AMordath::OnEnterLaughState()
{
	MordathAnimInstance->bCanLaugh = true;
}

void AMordath::UpdateLaughState()
{
}

void AMordath::OnExitLaughState()
{
	MordathAnimInstance->bCanLaugh = false;
}
#pragma endregion

#pragma region Dash
void AMordath::OnEnterDashState()
{
	DashComponent->StartCooldown();

	PlayAnimMontage(SuperCloseRange_ActionData->ActionMontage);
}

void AMordath::UpdateDashState()
{
	FacePlayer_Instant();

	if (!AnimInstance->Montage_IsPlaying(SuperCloseRange_ActionData->ActionMontage))
	{
		FSM->PopState();
	}
}

void AMordath::OnExitDashState()
{
	StopAnimMontage();
}
#pragma endregion

#pragma region Dash Combat
void AMordath::OnEnterDashCombatState()
{
	// Reset hit count
	HitCounter = 0;

	PlayActionMontage();
}

void AMordath::UpdateDashCombatState()
{
	FacePlayer();

	if (HasFinishedAction())
	{
		NextAction();
		
		FSM->PopState();
	}
}

void AMordath::OnExitDashCombatState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopActionMontage();
}

#pragma endregion

#pragma region Strafe
void AMordath::OnEnterStrafeState()
{
	PlayActionMontage();
}

void AMordath::UpdateStrafeState()
{
	FacePlayer();

	if (HasFinishedAction())
	{
		NextAction();
		
		FSM->PopState();
	}
}

void AMordath::OnExitStrafeState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopActionMontage();
}
#pragma endregion

#pragma region Tired
void AMordath::OnEnterTiredState()
{
	PlayActionMontage();
}

void AMordath::UpdateTiredState()
{
	if (HasFinishedAction())
		FSM->PopState();
}

void AMordath::OnExitTiredState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopActionMontage();

	NextAction();
}
#pragma endregion

#pragma region Back Hand
void AMordath::OnEnterBackHandState()
{
	PlayAnimMontage(CurrentStageData->Combat.BackHandActionData->ActionMontage);

	ActionDamage = CurrentStageData->Combat.BackHandActionData->ActionDamage;
}

void AMordath::UpdateBackHandState()
{
	FacePlayerBasedOnActionData(CurrentStageData->Combat.BackHandActionData);

	if (HasFinishedAction(CurrentStageData->Combat.BackHandActionData->ActionMontage))
	{
		FSM->PopState();
	}
}

void AMordath::OnExitBackHandState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAnimMontage(CurrentStageData->Combat.BackHandActionData->ActionMontage);

	ResetActionDamage();
}
#pragma endregion

#pragma region Teleport
void AMordath::OnEnterTeleportState()
{
	MordathAnimInstance->bCanTeleport = true;

	TeleportationComponent->GenerateTeleportTime();
}

void AMordath::UpdateTeleportState()
{
	const float Uptime = FSM->GetActiveStateUptime();

	if (Uptime > TeleportationComponent->GetTeleportTime() && !TeleportationComponent->IsCoolingDown())
	{
		if (CurrentActionData->bCanTeleportWithAction)
		{
			TeleportationComponent->StartCooldown();
			SetActorLocation(TeleportationComponent->FindLocationToTeleport(GameState->PlayerData.Location, GameState->GetTeleportRadius(), GameState->PlayArea));
		}

		FSM->PopState();
	}
}

void AMordath::OnExitTeleportState()
{
	MordathAnimInstance->bCanTeleport = false;
}
#pragma endregion  
#pragma endregion

#pragma region Boss Ranges
#pragma region Close Range
void AMordath::OnEnterCloseRange()
{
	GameState->PlayerData.CurrentRange = BRM_Close;
}

void AMordath::UpdateCloseRange()
{
	if (DistanceToPlayer < CurrentStageData->GetSuperCloseRangeRadius())
		RangeFSM->PushState("Super Close");

	if (DistanceToPlayer > CurrentStageData->GetCloseRangeRadius())
		RangeFSM->PushState("Mid");
}

void AMordath::OnExitCloseRange()
{
}
#pragma endregion 

#pragma region Mid Range
void AMordath::OnEnterMidRange()
{
	GameState->PlayerData.CurrentRange = BRM_Mid;
}

void AMordath::UpdateMidRange()
{
	if (DistanceToPlayer < CurrentStageData->GetCloseRangeRadius())
		RangeFSM->PushState("Close");

	if (DistanceToPlayer > CurrentStageData->GetMidRangeRadius())
		RangeFSM->PushState("Far");
}

void AMordath::OnExitMidRange()
{
}
#pragma endregion 

#pragma region Far Range
void AMordath::OnEnterFarRange()
{
	GameState->PlayerData.CurrentRange = BRM_Far;
}

void AMordath::UpdateFarRange()
{
	const float& Uptime = RangeFSM->GetActiveStateUptime();

	if (DistanceToPlayer < CurrentStageData->GetMidRangeRadius())
	{
		RangeFSM->PushState("Mid");
		return;
	}

	if (IsTeleporting() || IsSpecialAttacking())
		return;

	if (IsInFirstStage() && Uptime > CurrentStageData->Combat.FarRangeAttackDelay && !IsTired())
	{
		if (!IsRecovering())
			ExecuteAction(CurrentStageData->Combat.FarRangeActionData);
	}
	else if ((IsInSecondStage() || IsInThirdStage()) && Uptime > CurrentStageData->Combat.FarRangeAttackDelay && !IsTired())
	{
		const uint8 bWantsLongAttack = FMath::RandRange(0, 1);
		
		if (bWantsLongAttack)
		{
			if (!IsRecovering())
				ExecuteAction(CurrentStageData->Combat.FarRangeActionData);
		}
		else
		{
			if (CurrentActionData->bCanTeleportWithAction && !TeleportationComponent->IsCoolingDown() && !IsTired())
			{
				FSM->PopState();
				FSM->PushState("Teleport");
			}
		}
	}
}

void AMordath::OnExitFarRange()
{
}
#pragma endregion 

#pragma region Super Close
void AMordath::OnEnterSuperCloseRange()
{
	GameState->PlayerData.CurrentRange = BRM_SuperClose;

	IncreaseAttackDamage(CurrentStageData->GetAttackDamageMultiplier());
}

void AMordath::UpdateSuperCloseRange()
{
	if (RangeFSM->GetActiveStateUptime() > CurrentStageData->GetSuperCloseRangeTime() && (!IsDashing() && !IsAttacking() && !IsRecovering() && !IsStunned() && !IsKicking() && !IsTired() && !IsDoingBackHand() && !IsThinking()))
	{
		const uint8 bWantsKick = FMath::RandRange(0, 1);
		if (bWantsKick == 1 && IsInSecondStage())
		{
			FSM->PushState("Kick");
		}
		else if (!IsDashing() && !DashComponent->IsCooldownActive())
		{
			FSM->PushState("Dash");
		}
	}

	if (DistanceToPlayer > CurrentStageData->GetSuperCloseRangeRadius())
		RangeFSM->PopState();
}

void AMordath::OnExitSuperCloseRange()
{
	ResetActionDamage();
}
#pragma endregion 
#pragma endregion

#pragma region Boss Stages
#pragma region Stage 1
void AMordath::OnEnterFirstStage()
{
	CurrentStageData = StageOneData;

	MordathAnimInstance->CurrentStage = Stage_1;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[0];
}

void AMordath::UpdateFirstStage()
{
	// Can we enter the second stage?
	if (HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * SecondStageHealth)
	{
		GameState->BossData.OnEnterSecondStage.Broadcast();
		return;
	}

	if (ChosenCombo->IsAtLastAction() && !IsWaitingForNewCombo())
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
		if (!IsWaitingForNewCombo() && !IsDelayingAction())
		{
			ChooseAction();
		}
	}
}

void AMordath::OnExitFirstStage()
{
	if (!GameState->PlayerData.bIsDead)
		FSM->PopState();	
}
#pragma endregion 

#pragma region Stage 2
void AMordath::OnEnterSecondStage()
{
	CurrentStageData = StageTwoData;

	if (Stage2_Transition)
		PlayAnimMontage(Stage2_Transition);

	SuperCloseRange_ActionData = CurrentStageData->Combat.SuperCloseRangeActionData;

	MordathAnimInstance->CurrentStage = Stage_2;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];
}

void AMordath::UpdateSecondStage()
{
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

	if (ChosenCombo->IsAtLastAction() && !IsWaitingForNewCombo())
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
		if (!IsWaitingForNewCombo() && !IsDelayingAction())
			ChooseAction();
	}
}

void AMordath::OnExitSecondStage()
{
	if (!GameState->PlayerData.bIsDead)
		FSM->PopState();	
}
#pragma endregion 

#pragma region Stage 3
void AMordath::OnEnterThirdStage()
{
	CurrentStageData = StageThreeData;

	if (Stage3_Transition)
		PlayAnimMontage(Stage3_Transition);

	if (!GameState->PlayerData.bIsDead)
		FSM->PopState();	

	MordathAnimInstance->CurrentStage = Stage_3;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];
}

void AMordath::UpdateThirdStage()
{
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

	if (ChosenCombo->IsAtLastAction() && !IsWaitingForNewCombo())
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
		if (!IsWaitingForNewCombo() && !IsDelayingAction())
			ChooseAction();
	}
}

void AMordath::OnExitThirdStage()
{
}
#pragma endregion   
#pragma endregion

#pragma region Events
void AMordath::OnLowHealth()
{
	ChangeHitboxSize(CurrentStageData->GetAttackRadius() / 2.0f);
}

void AMordath::OnPlayerDeath()
{
	BossAIController->StopMovement();

	//FSM->RemoveAllStatesFromStack();
	//FSM->PushState("Laugh");

	RangeFSM->Stop();
	StageFSM->Stop();
}

void AMordath::OnAttackParryed()
{
	if ((CurrentActionData->Action->CounterType == ACM_Parryable || CurrentActionData->Action->CounterType == ACM_ParryableBlockable)  && !IsStunned())
	{
		PreviousActionMontage = AnimInstance->GetCurrentActiveMontage();
		PreviousActionMontage->BlendOut.SetBlendTime(CurrentStageData->GetAttackBlockedBlendOutTime());
		StopActionMontage();

		FSM->PopState();
		FSM->PushState("Stunned");

		// Shake the camera
		PlayerController->ClientPlayCameraShake(CurrentStageData->GetStunShake().Shake, CurrentStageData->GetStunShake().Intensity);
	}
}

void AMordath::OnAttackBlocked()
{
	if ((CurrentActionData->Action->CounterType == ACM_Blockable || CurrentActionData->Action->CounterType == ACM_ParryableBlockable) && !IsDamaged())
	{
		PreviousActionMontage = AnimInstance->GetCurrentActiveMontage();
		PreviousActionMontage->BlendOut.SetBlendTime(0.0f);
		StopActionMontage();

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
	const FVector NewLocation = CurrentLocation * FVector(1.0f, 1.0f, 0.0f);
	SpawnLightningStrike(NewLocation);

	StageFSM->PushState(1);
	StageFSM->PopState(0);

	CachedCombos.Empty();
	ChooseCombo();
}

void AMordath::OnThirdStageHealth()
{
	const FVector NewLocation = CurrentLocation * FVector(1.0f, 1.0f, 0.0f);
	SpawnLightningStrike(NewLocation);

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

void AMordath::PlayActionMontage()
{
	PlayAnimMontage(CurrentActionData->Action->ActionMontage);
}

void AMordath::PlayActionMontage(UMordathActionData* ActionData)
{
	PlayAnimMontage(ActionData->ActionMontage);
}

void AMordath::StopActionMontage()
{
	if (!GameState->IsPlayerDead())
		StopAnimMontage();

	CurrentActionData->bExecutionTimeExpired = false;

	CurrentMontageSection = "None";
	GameState->BossData.CurrentActionType = ATM_None;
	GameState->BossData.CurrentCounterType = ACM_None;
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
	// Determine the color to output based on damaged value
	TArray<int32> DamageValues;
	TArray<FLinearColor> ColorValues;
	for (auto It = DamageValueColor.CreateIterator(); It; ++It)
	{
		DamageValues.Add(It.Key());
		ColorValues.Add(It.Value());
	}

	for (int32 i = 0; i < DamageValues.Num(); ++i)
	{
		if (DamageValues.IsValidIndex(i+1))
		{
			if (DamageAmount >= DamageValues[i] && DamageAmount < DamageValues[i+1])
			{
				GameState->BossData.DamageColor = ColorValues[i];
				break;
			}
		}
		else
		{
			if (DamageAmount >= DamageValues[i])
			{
				GameState->BossData.DamageColor = ColorValues.Last();
				break;
			}
			
			GameState->BossData.DamageColor = ColorValues[0];
			break;
		}
	}

	UpdateDamageValueInMainHUD(DamageAmount);

	// Apply hit stop
	PauseAnimsWithTimer();

	UpdateHealth(DamageAmount);

	if (GameState->PlayerData.CurrentAttackType == ATP_Special)
	{
		FSM->PopState();
		FSM->PushState("Stunned");
	}

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

void AMordath::OnExecutionTimeExpired()
{
	CurrentActionData->bExecutionTimeExpired = true;
}

void AMordath::ChooseCombo()
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

			#if !UE_BUILD_SHIPPING
			if (Debug.bLogCurrentCombo)
				ULog::DebugMessage(SUCCESS, "Combo " + ChosenCombo->GetName() + " chosen", true);
			#endif

			ChosenCombo->Init();
			CurrentActionData = &ChosenCombo->GetCurrentActionData();
			
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
}

void AMordath::ChooseAction()
{
	if (IsAttacking())
		return;

	CurrentActionData = &ChosenCombo->GetCurrentActionData();

	UMordathActionData* ActionDataToUse = CurrentActionData->Action;

	if (CurrentActionData->bExecutionTimeExpired && CurrentActionData->FailSafeAction)
		ActionDataToUse = CurrentActionData->FailSafeAction;

	// Do a flash to indicate what kind of attack this is
	switch (ActionDataToUse->CounterType)
	{
	case ACM_Parryable:
		FlashIndicator->Flash(FlashIndicator->ParryableFlashColor);
	break;

	case ACM_Blockable:
		FlashIndicator->Flash(FlashIndicator->BlockableFlashColor);
	break;

	case ACM_ParryableBlockable:
		FlashIndicator->Flash(FlashIndicator->ParryableFlashColor);
	break;

	case ACM_NoCounter:
		FlashIndicator->Flash(FlashIndicator->NoCounterFlashColor);
	break;

	default:
	break;
	}

	// Update data
	GameState->BossData.CurrentActionType = CurrentActionData->Action->ActionType;
	GameState->BossData.CurrentCounterType = CurrentActionData->Action->CounterType;
	ActionDamage = CurrentActionData->Action->ActionDamage;

	ExecuteAction(ActionDataToUse);
}

void AMordath::NextAction()
{
	if (ChosenCombo->IsDelayEnabled() && !IsDelayingAction())
	{
		const float Min = FMath::Clamp(ChosenCombo->GetActionDelayTime() - ChosenCombo->GetDeviation(), 0.0f, 100.0f);
		const float Max = FMath::Clamp(ChosenCombo->GetActionDelayTime() + ChosenCombo->GetDeviation(), 0.0f, 100.0f + ChosenCombo->GetDeviation());
		const float NewDelay = FMath::RandRange(Min, Max);

		if (NewDelay > 0.0f)
		{
			TimerManager->SetTimer(ChosenCombo->GetActionDelayTimer(), this, &AMordath::NextAction, NewDelay);
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

void AMordath::UpdateDamageValueInMainHUD(const float DamageAmount) const
{
	MainHUD->FadeInDamageValue();
	MainHUD->UpdateDamageValue(DamageAmount);
}

void AMordath::ExecuteAction(UMordathActionData* ActionData)
{
	StopMoving();

	CurrentActionData->Action = ActionData;

	FSM->PushState("Action");
}

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead
	if (FSM->GetActiveStateName() == "Death" || AnimInstance->bIsHit || DamageCauser->IsA(AMordathGhost::StaticClass()))
		return DamageAmount;

	BeginTakeDamage(DamageAmount);

	// Apply damage once
	if (!AnimInstance->bIsHit /*&& HitCounter < CurrentStageData->Combat.MaxHitsBeforeInvincibility && !TimerManager->IsTimerActive(TH_Invincibility)*/)
	{
		ApplyDamage(DamageAmount);
	}

	// When we have reached the maximum amount of hits we can tolerate, enable invincibility
	//if (HitCounter == CurrentStageData->Combat.MaxHitsBeforeInvincibility && !TimerManager->IsTimerActive(TH_Invincibility))
	//{
	//	// Reset our hits
	//	HitCounter = 0;
	//
	//	// Become invincible and set a timer to disable invincibility after 'X' seconds
	//	//EnableInvincibility();
	//	//TimerManager->SetTimer(TH_Invincibility, this, &AMordath::DisableInvincibility, CurrentStageData->Combat.InvincibilityTimeAfterDamage);
	//
	//	// Cancel our current animation and enter the downed state
	//	//FSM->PushState("Beaten");
	//
	//	UpdateDamageValueInMainHUD(DamageAmount);
	//
	//	UpdateHealth(DamageAmount);
	//
	//	// Handled in blueprints
	//	OnAfterTakeDamage();
	//}

	EndTakeDamage();

	return DamageAmount;
}

void AMordath::ChangeHitboxSize(const float NewRadius)
{
}

void AMordath::FacePlayer(const float RotationSpeed)
{
	if (RotationSpeed > 0.0f)
		SetActorRotation(FMath::Lerp(GetControlRotation(), FRotator(GetControlRotation().Pitch, DirectionToPlayer.Rotation().Yaw, GetControlRotation().Roll), RotationSpeed * World->DeltaTimeSeconds));
}

void AMordath::FacePlayer_Instant()
{
	SetActorRotation(FRotator(GetControlRotation().Pitch, DirectionToPlayer.Rotation().Yaw, GetControlRotation().Roll));
}

void AMordath::FacePlayerBasedOnActionData(const class UMordathActionData* ActionData)
{
	CurrentMontageSection = AnimInstance->Montage_GetCurrentSection(ActionData->ActionMontage);

	const auto SnapToPlayerLocation = [&](const FMontageActionData_Base& MontageActionData)
	{
		FVector NewLocation;

		if (MontageActionData.bSmoothSnap)
		{
			switch (MontageActionData.BlendOption)
			{
			case EAlphaBlendOption::Linear:
				NewLocation = FMath::Lerp(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::Cubic:
				NewLocation = FMath::CubicInterp(CurrentLocation, CurrentLocation.GetSafeNormal(), GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, GameState->PlayerData.Location.GetSafeNormal(), MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::Sinusoidal:
				NewLocation = FMath::InterpSinIn(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::QuadraticInOut:
				NewLocation = FMath::InterpEaseInOut(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds, 2);
			break;

			case EAlphaBlendOption::QuarticInOut:
				NewLocation = FMath::InterpEaseInOut(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds, 4);
			break;

			case EAlphaBlendOption::QuinticInOut:
				NewLocation = FMath::InterpEaseInOut(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds, 5);
			break;

			case EAlphaBlendOption::CircularIn:
				NewLocation = FMath::InterpCircularIn(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::CircularOut:
				NewLocation = FMath::InterpCircularOut(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::CircularInOut:
				NewLocation = FMath::InterpCircularInOut(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::ExpIn:
				NewLocation = FMath::InterpExpoIn(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::ExpOut:
				NewLocation = FMath::InterpExpoOut(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			case EAlphaBlendOption::ExpInOut:
				NewLocation = FMath::InterpExpoInOut(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;

			default:
				NewLocation = FMath::Lerp(CurrentLocation, GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer, MontageActionData.Speed * World->DeltaTimeSeconds);
			break;
			}
			
		}
		else
			NewLocation = GameState->PlayerData.Location - GetActorForwardVector() * MontageActionData.DistanceFromPlayer;

		NewLocation.Z = CurrentLocation.Z;
		SetActorLocation(NewLocation);
	};

	if (CurrentMontageSection == "Anticipation")
	{
		FacePlayer(ActionData->Anticipation.RotationSpeed);
		AnimInstance->Montage_SetPlayRate(ActionData->ActionMontage, ActionData->Anticipation.PlayRate);

		if (ActionData->Anticipation.bSnapToPlayerLocation)
		{
			SnapToPlayerLocation(ActionData->Anticipation);
		}
	}
	else if (CurrentMontageSection == "Pinnacle")
	{
		if (ActionData->Pinnacle.bSnapToPlayerLocation)
		{
			SnapToPlayerLocation(ActionData->Pinnacle);
		}

		if (ActionData->Pinnacle.bSnapRotation)
			FacePlayer_Instant();
	}
	else if (CurrentMontageSection == "Contact")
	{
		FacePlayer(ActionData->Contact.RotationSpeed);
		AnimInstance->Montage_SetPlayRate(ActionData->ActionMontage, ActionData->Contact.PlayRate);

		if (ActionData->Contact.bSnapToPlayerLocation)
		{
			SnapToPlayerLocation(ActionData->Contact);
		}
	}
	else if (CurrentMontageSection == "Recovery")
	{
		FacePlayer(ActionData->Recovery.RotationSpeed);
		AnimInstance->Montage_SetPlayRate(ActionData->ActionMontage, ActionData->Recovery.PlayRate);

		if (ActionData->Recovery.bSnapToPlayerLocation)
		{
			SnapToPlayerLocation(ActionData->Recovery);
		}
	}
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

bool AMordath::IsKicking() const
{
	return FSM->GetActiveStateID() == 20;
}

void AMordath::ChooseMovementDirection()
{
	GWantsMoveRight = FMath::RandBool();
}

void AMordath::EncirclePlayer()
{
	if (PlayerCharacter->GetInputAxisValue("MoveRight") > 0.0f && PlayerCharacter->HasMovedRightBy(300.0f))
	{
		MoveRight(-1.0f);
	}
	else if (PlayerCharacter->GetInputAxisValue("MoveRight") < 0.0f && PlayerCharacter->HasMovedLeftBy(300.0f))
	{
		MoveRight();
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
	return (CurrentActionData->RangeToExecute == RangeFSM->GetActiveStateID() || 
			CurrentActionData->RangeToExecute == BRM_AnyRange || 
			IsExecutionTimeExpired() || 
			ChosenCombo->WantsToExecuteNonStop()) &&
			!IsRecovering() && !IsAttacking() && !IsDashing() && !IsTransitioning() && !IsStunned() && !IsDamaged() && !IsStrafing();
}

void AMordath::ResetActionDamage()
{
	ActionDamage = CurrentActionData->Action->ActionDamage;
}

void AMordath::IncreaseAttackDamage(const float& Multiplier)
{
	ActionDamage *= Multiplier;
}

void AMordath::StopMoving()
{
	CurrentMovementSpeed = 0.0f;
	ForwardInput = 0.0f;
	RightInput = 0.0f;
}

void AMordath::StartExecutionExpiryTimer()
{
	TimerManager->SetTimer(CurrentActionData->TH_ExecutionExpiry, this, &AMordath::OnExecutionTimeExpired, CurrentActionData->ExecutionTime, false);
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
	return FSM->GetActiveStateID() == 25 && 
			GameState->BossData.CurrentActionType == ATM_ShortAttack_1 || 
			GameState->BossData.CurrentActionType == ATM_ShortAttack_2 || 
			GameState->BossData.CurrentActionType == ATM_ShortAttack_3 /* FSM->GetActiveStateID() == 3 || FSM->GetActiveStateID() == 4 || FSM->GetActiveStateID() == 5*/;
}

bool AMordath::IsLongAttacking() const
{
	return FSM->GetActiveStateID() == 25 && 
			GameState->BossData.CurrentActionType == ATM_LongAttack_1 || 
			GameState->BossData.CurrentActionType == ATM_LongAttack_2 || 
			GameState->BossData.CurrentActionType == ATM_LongAttack_3/*FSM->GetActiveStateID() == 6 || FSM->GetActiveStateID() == 7 || FSM->GetActiveStateID() == 8*/;
}

bool AMordath::IsSpecialAttacking() const
{
	return FSM->GetActiveStateID() == 25 && 
			GameState->BossData.CurrentActionType == ATM_SpecialAttack_1 || 
			GameState->BossData.CurrentActionType == ATM_SpecialAttack_2 || 
			GameState->BossData.CurrentActionType == ATM_SpecialAttack_3 /*FSM->GetActiveStateID() == 9 || FSM->GetActiveStateID() == 10 || FSM->GetActiveStateID() == 11*/;
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
	return TimerManager->IsTimerActive(TH_ComboDelay);
}

bool AMordath::IsDelayingAction() const
{
	return TimerManager->IsTimerActive(ChosenCombo->GetActionDelayTimer());
}

bool AMordath::IsDashing() const
{
	return FSM->GetActiveStateID() == 16 || FSM->GetActiveStateID() == 17 || FSM->GetActiveStateID() == 25 && (CurrentActionData->Action->ActionType == ATM_Dash_Backward || CurrentActionData->Action->ActionType == ATM_Dash_Forward || CurrentActionData->Action->ActionType == ATM_Dash_Left || CurrentActionData->Action->ActionType == ATM_Dash_Right);
}

bool AMordath::IsStrafing() const
{
	return FSM->GetActiveStateID() == 22;
}

bool AMordath::IsDamaged() const
{
	return FSM->GetActiveStateID() == 12;
}

bool AMordath::IsThinking() const
{
	return FSM->GetActiveStateID() == 2;
}

bool AMordath::WantsMoveRight() const
{
	return GWantsMoveRight == 1;
}

bool AMordath::IsRecovering() const
{
	return FSM->GetActiveStateID() == 21;
}

bool AMordath::HasFinishedAction() const
{
	return !AnimInstance->Montage_IsPlaying(CurrentActionData->Action->ActionMontage);
}

bool AMordath::HasFinishedAction(UAnimMontage* ActionMontage) const
{
	return !AnimInstance->Montage_IsPlaying(ActionMontage);
}

bool AMordath::IsTransitioning() const
{
	return AnimInstance->Montage_IsPlaying(Stage2_Transition) || AnimInstance->Montage_IsPlaying(Stage3_Transition);
}

bool AMordath::IsTired() const
{
	return FSM->GetActiveStateID() == 25 && CurrentActionData->Action->ActionType == ATM_Tired;
}

bool AMordath::IsDoingBackHand() const
{
	return FSM->GetActiveStateID() == 24;
}

bool AMordath::IsTeleporting() const
{
	return FSM->GetActiveStateID() == 18;
}

bool AMordath::IsExecutionTimeExpired() const
{
	return !TimerManager->IsTimerActive(CurrentActionData->TH_ExecutionExpiry);
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

float AMordath::GetActionDamage() const
{
	return ActionDamage;
}

float AMordath::GetShortAttackDamage() const
{
	return 0.0f;
}

float AMordath::GetLongAttackDamage() const
{
	return 0.0f;
}

float AMordath::GetSpecialAttackDamage() const
{
	return 0.0f;
}

float AMordath::GetAttackRadius() const
{
	return CurrentStageData->GetAttackRadius();
}

float AMordath::GetRecentDamage() const
{
	return CurrentStageData->GetRecentDamage();
}

void AMordath::EnterStage(const EBossStage_Mordath InStage)
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

UForceFeedbackEffect* AMordath::GetCurrentForceFeedbackEffect() const
{
	return CurrentActionData->Action->ForceFeedbackEffect;
}

void AMordath::SpawnLightningStrike(const FVector& LocationToSpawn, const FRotator& Rotation)
{
	World->SpawnActor(LightningStrikeClass, &LocationToSpawn, &Rotation);
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
	OverthroneHUD->AddOnScreenDebugMessage("Current Attack: ", FColor::Yellow, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Current Counter: ", FColor::Yellow, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Lock-on Location: ", FColor::Green, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Action Damage: ", FColor::Green, YPadding);
	OverthroneHUD->AddOnScreenDebugMessage("Current Combo: ", FColor::Yellow, YPadding);
}
