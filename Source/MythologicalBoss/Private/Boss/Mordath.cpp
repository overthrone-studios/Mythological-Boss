// Copyright Overthrone Studios 2019

#include "Mordath.h"

#include "OverthroneFunctionLibrary.h"
#include "OverthroneGameInstance.h"
#include "OverthroneGameState.h"
#include "OverthroneHUD.h"
#include "OverthroneEnums.h"

#include "Potions/PotionBase.h"

#include "Misc/MordathStageData.h"
#include "Misc/MordathDifficultyData.h"
#include "Misc/BoundingBox.h"

#include "FSM.h"
#include "Log.h"

#include "Boss/BossAIController.h"
#include "Boss/MordathAnimInstance.h"
#include "Boss/MordathGhost.h"
#include "Boss/MordathFeatherComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TeleportationComponent.h"
#include "Components/DashComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "Animation/AnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "HUD/MasterHUD.h"
#include "HUD/MainPlayerHUD.h"
#include "HUD/FSMVisualizerHUD.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "DamageTypes/DmgType_MordathElectricShield.h"

#include "Engine/StaticMesh.h"

#include "TimerManager.h"

AMordath::AMordath() : AMordathBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Add more states
	FSM->AddState(12, "Damaged");
	FSM->AddState(13, "Transition");
	FSM->AddState(14, "Stunned");
	FSM->AddState(15, "Roar");
	FSM->AddState(16, "Dash");
	FSM->AddState(17, "Dash Combat");
	FSM->AddState(18, "Teleport");
	FSM->AddState(19, "Retreat");
	FSM->AddState(20, "Kick");
	FSM->AddState(21, "Recover");
	FSM->AddState(23, "Tired");
	FSM->AddState(26, "Close Action");
	FSM->AddState(27, "Far Action");
	FSM->AddState(28, "Invincible");

	// Bind state events to our functions
	FSM->GetStateFromID(12)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDamagedState);
	FSM->GetStateFromID(12)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDamagedState);
	FSM->GetStateFromID(12)->OnExitState.AddDynamic(this, &AMordath::OnExitDamagedState);

	FSM->GetStateFromID(13)->OnEnterState.AddDynamic(this, &AMordath::OnEnterTransitionState);
	FSM->GetStateFromID(13)->OnUpdateState.AddDynamic(this, &AMordath::UpdateTransitionState);
	FSM->GetStateFromID(13)->OnExitState.AddDynamic(this, &AMordath::OnExitTransitionState);

	FSM->GetStateFromID(14)->OnEnterState.AddDynamic(this, &AMordath::OnEnterStunnedState);
	FSM->GetStateFromID(14)->OnUpdateState.AddDynamic(this, &AMordath::UpdateStunnedState);
	FSM->GetStateFromID(14)->OnExitState.AddDynamic(this, &AMordath::OnExitStunnedState);

	FSM->GetStateFromID(15)->OnEnterState.AddDynamic(this, &AMordath::OnEnterRoarState);
	FSM->GetStateFromID(15)->OnUpdateState.AddDynamic(this, &AMordath::UpdateRoarState);
	FSM->GetStateFromID(15)->OnExitState.AddDynamic(this, &AMordath::OnExitRoarState);

	FSM->GetStateFromID(16)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDashState);
	FSM->GetStateFromID(16)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDashState);
	FSM->GetStateFromID(16)->OnExitState.AddDynamic(this, &AMordath::OnExitDashState);

	FSM->GetStateFromID(17)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDashCombatState);
	FSM->GetStateFromID(17)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDashCombatState);
	FSM->GetStateFromID(17)->OnExitState.AddDynamic(this, &AMordath::OnExitDashCombatState);

	FSM->GetStateFromID(18)->OnEnterState.AddDynamic(this, &AMordath::OnEnterTeleportState);
	FSM->GetStateFromID(18)->OnUpdateState.AddDynamic(this, &AMordath::UpdateTeleportState);
	FSM->GetStateFromID(18)->OnExitState.AddDynamic(this, &AMordath::OnExitTeleportState);

	FSM->GetStateFromID(19)->OnEnterState.AddDynamic(this, &AMordath::OnEnterRetreatState);
	FSM->GetStateFromID(19)->OnUpdateState.AddDynamic(this, &AMordath::UpdateRetreatState);
	FSM->GetStateFromID(19)->OnExitState.AddDynamic(this, &AMordath::OnExitRetreatState);

	FSM->GetStateFromID(20)->OnEnterState.AddDynamic(this, &AMordath::OnEnterKickState);
	FSM->GetStateFromID(20)->OnUpdateState.AddDynamic(this, &AMordath::UpdateKickState);
	FSM->GetStateFromID(20)->OnExitState.AddDynamic(this, &AMordath::OnExitKickState);

	FSM->GetStateFromID(21)->OnEnterState.AddDynamic(this, &AMordath::OnEnterRecoverState);
	FSM->GetStateFromID(21)->OnUpdateState.AddDynamic(this, &AMordath::UpdateRecoverState);
	FSM->GetStateFromID(21)->OnExitState.AddDynamic(this, &AMordath::OnExitRecoverState);

	FSM->GetStateFromID(23)->OnEnterState.AddDynamic(this, &AMordath::OnEnterTiredState);
	FSM->GetStateFromID(23)->OnUpdateState.AddDynamic(this, &AMordath::UpdateTiredState);
	FSM->GetStateFromID(23)->OnExitState.AddDynamic(this, &AMordath::OnExitTiredState);

	FSM->GetStateFromID(26)->OnEnterState.AddDynamic(this, &AMordath::OnEnterCloseActionState);
	FSM->GetStateFromID(26)->OnUpdateState.AddDynamic(this, &AMordath::UpdateCloseActionState);
	FSM->GetStateFromID(26)->OnExitState.AddDynamic(this, &AMordath::OnExitCloseActionState);

	FSM->GetStateFromID(27)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFarActionState);
	FSM->GetStateFromID(27)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFarActionState);
	FSM->GetStateFromID(27)->OnExitState.AddDynamic(this, &AMordath::OnExitFarActionState);

	FSM->GetStateFromID(28)->OnEnterState.AddDynamic(this, &AMordath::OnEnterInvincibleState);
	FSM->GetStateFromID(28)->OnUpdateState.AddDynamic(this, &AMordath::UpdateInvincibleState);
	FSM->GetStateFromID(28)->OnExitState.AddDynamic(this, &AMordath::OnExitInvincibleState);

	FSM->InitFSM(0);

	// Create a stage FSM
	StageFSM = CreateDefaultSubobject<UFSM>(FName("Stage FSM"));
	StageFSM->AddState(0, "Stage 1");
	StageFSM->AddState(1, "Stage 2");
	StageFSM->AddState(2, "Stage 3");

	StageFSM->OnEnterAnyState.AddDynamic(this, &AMordath::OnEnterAnyStageState);
	StageFSM->OnUpdateAnyState.AddDynamic(this, &AMordath::UpdateAnyStageState);
	StageFSM->OnExitAnyState.AddDynamic(this, &AMordath::OnExitAnyStageState);

	StageFSM->GetStateFromID(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterFirstStage);
	StageFSM->GetStateFromID(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateFirstStage);
	StageFSM->GetStateFromID(0)->OnExitState.AddDynamic(this, &AMordath::OnExitFirstStage);

	StageFSM->GetStateFromID(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterSecondStage);
	StageFSM->GetStateFromID(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateSecondStage);
	StageFSM->GetStateFromID(1)->OnExitState.AddDynamic(this, &AMordath::OnExitSecondStage);

	StageFSM->GetStateFromID(2)->OnEnterState.AddDynamic(this, &AMordath::OnEnterThirdStage);
	StageFSM->GetStateFromID(2)->OnUpdateState.AddDynamic(this, &AMordath::UpdateThirdStage);
	StageFSM->GetStateFromID(2)->OnExitState.AddDynamic(this, &AMordath::OnExitThirdStage);

	StageFSM->InitFSM(0);

	// Teleportation component
	TeleportationComponent = CreateDefaultSubobject<UTeleportationComponent>(FName("Teleportation Component"));
	TeleportationComponent->OnDisappeared.AddDynamic(this, &AMordath::OnDisappeared);
	TeleportationComponent->OnReappeared.AddDynamic(this, &AMordath::OnReappeared);
	TeleportationComponent->OnBeginDisappear.AddDynamic(this, &AMordath::OnBeginDisappear);
	TeleportationComponent->OnBeginReappear.AddDynamic(this, &AMordath::OnBeginReappear);

	// Dash component
	DashComponent = CreateDefaultSubobject<UDashComponent>(FName("Dash Component"));

	HealthComponent->OnFullHealth.AddDynamic(this, &AMordath::OnFullHealth);

	// Energy shield collision component
	EnergyShieldCollision = CreateDefaultSubobject<USphereComponent>(FName("Energy Shield Collision"));
	EnergyShieldCollision->SetupAttachment(RootComponent);
	EnergyShieldCollision->SetSphereRadius(200.0f);
	EnergyShieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AMordath::OnEnterEnergySphere);
	EnergyShieldCollision->OnComponentEndOverlap.AddDynamic(this, &AMordath::OnExitEnergySphere);

	Tags.Empty();
	Tags.Add("Mordath-Main");
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("BossFSMVisualizer"));

	CurrentDifficultyData = GetDifficultyData();

	SKM_Feathers = GetFeathers();
	SKM_ElectricShield = GetShield();
	SKM_ElectricShield->SetVisibility(false);

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
	GameState->BossData.LockOnBoneName = LockOnBoneName;
	GameState->Mordath = this;
	UpdateCharacterInfo();

	OriginalMaterial = SKMComponent->GetMaterial(0);
	MID_OriginalMaterial = SKMComponent->CreateDynamicMaterialInstance(0, OriginalMaterial, FName("MID_Mordath"));

	SKM_Feathers->SetMaterial(0, MID_OriginalMaterial);

	const FMaterialParameterInfo MPC_Mordath{"Attack Color"};
	MID_OriginalMaterial->GetVectorParameterValue(MPC_Mordath, OriginalAttackColor);

	AnimInstance->OnMontageEnded.AddDynamic(this, &AMordath::OnAttackEnd_Implementation);
	OnEnterPerfectDash.AddDynamic(this, &AMordath::OnEnterPerfectDashWindow);

	// Begin the state machines
	FSM->Start();
	RangeFSM->Start();
	StageFSM->Start();

	ChooseCombo();

	ResetActionDamage();

	MainHUD->ChangeBossHealthBarColor(BossDefaultHealth_BarColor);

	HealthComponent->SetDefaultHealth(CurrentDifficultyData->DefaultHealth);
	HealthComponent->SetHealth(CurrentDifficultyData->DefaultHealth);
	UpdateCharacterInfo();

#if !UE_BUILD_SHIPPING
	if (Debug.bShowRaycasts)
	{
		CapsuleComp->SetHiddenInGame(false);
		EnergyShieldCollision->SetHiddenInGame(false);
	}
	else
	{
		CapsuleComp->SetHiddenInGame(true);
		EnergyShieldCollision->SetHiddenInGame(true);
	}
#else
	CapsuleComp->SetHiddenInGame(true);
	EnergyShieldCollision->SetHiddenInGame(true);
#endif
}

void AMordath::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameState->BossData.Location = CurrentLocation;

#if !UE_BUILD_SHIPPING
	if (Debug.bShowRaycasts)
	{
		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetSuperCloseRangeRadius(), 32, FColor::Red, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetCloseRangeRadius(), 32, FColor::Orange, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetMidRangeRadius(), 32, FColor::Cyan, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);

		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), CurrentStageData->GetMidRangeRadius() * 2, 32, FColor::Green, 0.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);
	}

	const int32& TotalMessages = OverthroneHUD->GetDebugMessagesCount();

	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 13, "Boss Health: " + FString::SanitizeFloat(HealthComponent->GetCurrentHealth()));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 12, "Boss Forward Input: " + FString::SanitizeFloat(ForwardInput));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 11, "Boss Right Input: " + FString::SanitizeFloat(RightInput));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 10, "Current Montage Section: " + CurrentMontageSection.ToString());
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 9, "Movement Speed: " + FString::SanitizeFloat(CurrentMovementSpeed));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 8, "Distance To Player: " + FString::SanitizeFloat(DistanceToPlayer));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 7, "Direction To Player: " + FString::SanitizeFloat(DirectionToPlayer.Rotation().Yaw));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 6, "Current Action: " + UOverthroneEnums::MordathAttackTypeToString(GameState->BossData.CurrentActionType)/*CurrentActionData->Action->GetCurrentActionAsString()*/);
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 5, "Current Counter: " + UOverthroneEnums::MordathAttackCounterTypeToString(GameState->BossData.CurrentCounterType) /*CurrentActionData->Action->GetCounterTypeAsString()*/);
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 4, "Lock-on Location Z: " + FString::SanitizeFloat(GameState->LockOnLocation.Z));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 3, "Action Damage: " + FString::SanitizeFloat(GetActionDamage()));
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 2, "Current Combo: " + ChosenCombo->GetName());
	OverthroneHUD->UpdateOnScreenDebugMessage(TotalMessages - 1, "Current Action Montage: " + CurrentMontageName);
#endif
}

#pragma region Any States
void AMordath::OnEnterAnyState(int32 ID, FName Name)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->HighlightState(Name.ToString());
}

void AMordath::UpdateAnyState(int32 ID, FName Name, float Uptime, int32 Frames)
{
	Super::UpdateAnyState(ID, Name, Uptime, Frames);

	if (!FSMVisualizer)
		return;

	FSMVisualizer->UpdateStateUptime(Name.ToString(), Uptime);
	FSMVisualizer->UpdateStateFrames(Name.ToString(), Frames);
}

void AMordath::OnExitAnyState(int32 ID, FName Name)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->UnhighlightState(Name.ToString());

	FSMVisualizer->UpdatePreviousStateUptime(Name.ToString(), FSM->GetActiveStateUptime());
	FSMVisualizer->UpdatePreviousStateFrames(Name.ToString(), FSM->GetActiveStateFrames());

	FSMVisualizer->UpdatePreviousState(Name.ToString(), FSM->GetActiveStateUptime(), FSM->GetActiveStateFrames());
}

void AMordath::OnEnterAnyRangeState(int32 ID, FName Name)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->HighlightState(Name.ToString());
}

void AMordath::UpdateAnyRangeState(int32 ID, FName Name, float Uptime, int32 Frames)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->UpdateStateUptime(Name.ToString(), Uptime);
	FSMVisualizer->UpdateStateFrames(Name.ToString(), Frames);
}

void AMordath::OnExitAnyRangeState(int32 ID, FName Name)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->UnhighlightState(Name.ToString());

	FSMVisualizer->UpdatePreviousStateUptime(Name.ToString(), RangeFSM->GetActiveStateUptime());
	FSMVisualizer->UpdatePreviousStateFrames(Name.ToString(), RangeFSM->GetActiveStateFrames());
}

void AMordath::OnEnterAnyStageState(int32 ID, FName Name)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->HighlightState(Name.ToString());
}

void AMordath::UpdateAnyStageState(int32 ID, FName Name, float Uptime, int32 Frames)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->UpdateStateUptime(Name.ToString(), Uptime);
	FSMVisualizer->UpdateStateFrames(Name.ToString(), Frames);
}

void AMordath::OnExitAnyStageState(int32 ID, FName Name)
{
	if (!FSMVisualizer)
		return;

	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());

	FSMVisualizer->UpdatePreviousStateUptime(Name.ToString(), StageFSM->GetActiveStateUptime());
	FSMVisualizer->UpdatePreviousStateFrames(Name.ToString(), StageFSM->GetActiveStateFrames());
}
#pragma endregion

#pragma region Boss States
#pragma region Follow
void AMordath::OnEnterFollowState()
{
	Super::OnEnterFollowState();
}

void AMordath::UpdateFollowState(float Uptime, int32 Frames)
{
	Super::UpdateFollowState(Uptime, Frames);

	if (IsWaitingForNextCombo() && IsSuperCloseRange())
	{
		FSM->PushState("Retreat");
		return;
	}

	if (IsWaitingForNextCombo() && IsCloseRange())
	{
		FSM->PushState("Thinking");
	}
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

void AMordath::UpdateRetreatState(float Uptime, int32 Frames)
{
	if (IsTransitioning())
	{
		BossAIController->StopMovement();

		return;
	}

	FacePlayer();

	if (DistanceToPlayer > CurrentStageData->GetMidRangeRadius())
		FSM->PopState();

	if (IsWaitingForNextCombo() && DistanceToPlayer < CurrentStageData->GetCloseRangeRadius() || Uptime <= RetreatTime)
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

void AMordath::UpdateKickState(float Uptime, int32 Frames)
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

void AMordath::UpdateRecoverState(float Uptime, int32 Frames)
{
	if (Uptime > CurrentStageData->GetRecoverTime())
	{
		FSM->PopState();
	}
}

void AMordath::OnExitRecoverState()
{
	MordathAnimInstance->bIsRecovering = false;
}
#pragma endregion  

#pragma region Invincible
void AMordath::OnEnterInvincibleState()
{
	MordathAnimInstance->bIsInvincible = true;

	SKM_ElectricShield->SetVisibility(true);
	EnableInvincibility();

	MainHUD->ChangeBossHealthBarColor(BossInvincibleHealth_BarColor);

	OnEnergyShieldActivated();
}

void AMordath::UpdateInvincibleState(float Uptime, int32 Frames)
{
	StopMovement();

	if (bRegenerateWhileInvincible)
		IncreaseHealth(RegenerationAmount);

	if (bHasActorEnteredEnergySphere)
		CharacterInEnergySphere->TakeDamage(1.0f, FDamageEvent(UDmgType_MordathElectricShield::StaticClass()), Controller, this);

	if (GameState->Mordaths.Num() < MaxGhosts + 1)
		FSM->PopState();
}

void AMordath::OnExitInvincibleState()
{
	MordathAnimInstance->bIsInvincible = false;

	SKM_ElectricShield->SetVisibility(false);
	DisableInvincibility();

	MainHUD->ChangeBossHealthBarColor(BossDefaultHealth_BarColor);

	OnEnergyShieldDeactivated();
}
#pragma endregion  

#pragma region Locked
void AMordath::OnEnterLockedState()
{
	Super::OnEnterLockedState();

	if (!OverthroneHUD)
		OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	OverthroneHUD->GetMasterHUD()->HighlightBox(18);
}

void AMordath::UpdateLockedState(float Uptime, int32 Frames)
{
	Super::UpdateLockedState(Uptime, Frames);
}

void AMordath::OnExitLockedState()
{
	if (!OverthroneHUD)
		OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	OverthroneHUD->GetMasterHUD()->UnhighlightBox(18);
}
#pragma endregion  

#pragma region Think
void AMordath::OnEnterThinkState()
{
	Super::OnEnterThinkState();

	ThinkTime = CurrentStageData->ThinkStateData.CalculateThinkTime();

	#if !UE_BUILD_SHIPPING
	if (Debug.bLogThinkTime)
		ULog::Number(ThinkTime, "Think Time: ", true);
	#endif
}

void AMordath::UpdateThinkState(const float Uptime, const int32 Frames)
{
	Super::UpdateThinkState(Uptime, Frames);

	if (!IsWaitingForNextCombo() && Uptime > ThinkTime)
	{
		FSM->PopState();
		FSM->PushState("Follow");
	}
}

void AMordath::OnExitThinkState()
{
	Super::OnExitThinkState();
}
#pragma endregion 

#pragma region Action
void AMordath::OnEnterActionState()
{
	Super::OnEnterActionState();

	OnBeginExecuteAction();
}

void AMordath::UpdateActionState(const float Uptime, const int32 Frames)
{
	Super::UpdateActionState(Uptime, Frames);

	GameState->BossData.bHasAttackBegun = CurrentActionData->Action->bCanBeDodged;
}

void AMordath::OnExitActionState()
{
	MID_OriginalMaterial->SetVectorParameterValue("Attack Color", OriginalAttackColor);

	OnEndExecuteAction();

	if (CurrentActionData->bExecutionTimeExpired)
	{
		StopActionMontage();
		NextAction();

		return;
	}

	Super::OnExitActionState();
}
#pragma endregion 

#pragma region Close Action
void AMordath::OnEnterCloseActionState()
{
	SuperCloseRange_ActionData = CurrentStageData->GetRandomSuperCloseRangeAction();

	if (!SuperCloseRange_ActionData)
	{
		ULog::Error("There are no super close range actions to choose from!", true);
		FSM->PopState();
		return;
	}

	PlayActionMontage(SuperCloseRange_ActionData);

	CurrentActionType = SuperCloseRange_ActionData->ActionType;
	CurrentCounterType = SuperCloseRange_ActionData->CounterType;
	GameState->BossData.CurrentActionType = SuperCloseRange_ActionData->ActionType;
	GameState->BossData.CurrentCounterType = SuperCloseRange_ActionData->CounterType;

	StartActionLocation = CurrentLocation;

	OnBeginExecuteAction();
}

void AMordath::UpdateCloseActionState(float Uptime, int32 Frames)
{
	StopMovement();

	if (SuperCloseRange_ActionData->bConstantlyFacePlayer)
		FacePlayer();
	else
		FacePlayerBasedOnActionData(SuperCloseRange_ActionData);

	if (AnimInstance->Montage_GetPosition(SuperCloseRange_ActionData->ActionMontage) >= SuperCloseRange_ActionData->MinPerfectDashWindow &&
		AnimInstance->Montage_GetPosition(SuperCloseRange_ActionData->ActionMontage) <= SuperCloseRange_ActionData->MaxPerfectDashWindow &&
		SuperCloseRange_ActionData->bAllowPerfectDash)
	{
		SuperCloseRange_ActionData->bCanBeDodged = true;

		if (!bPerfectDashEmitterSpawned)
		{
			OnEnterPerfectDash.Broadcast();
			bPerfectDashEmitterSpawned = true;
		}
	}
	else
	{
		SuperCloseRange_ActionData->bCanBeDodged = false;
	}

	GameState->BossData.bHasAttackBegun = SuperCloseRange_ActionData->bCanBeDodged;

	// If action has finished, go back to previous state
	if (HasFinishedAction(SuperCloseRange_ActionData->ActionMontage))
		FSM->PopState();
}

void AMordath::OnExitCloseActionState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAnimMontage();

	bPerfectDashEmitterSpawned = false;

	CurrentMontageSection = "None";
	CurrentMontageName = "None";

	CurrentActionType = ATM_None;
	CurrentCounterType = ACM_None;
	GameState->BossData.CurrentActionType = ATM_None;
	GameState->BossData.CurrentCounterType = ACM_None;

	SuperCloseRange_ActionData->bCanBeDodged = false;
	GameState->BossData.bHasAttackBegun = false;

	MID_OriginalMaterial->SetVectorParameterValue("Attack Color", OriginalAttackColor);

	OnEndExecuteAction();
}
#pragma endregion 

#pragma region Far Action
void AMordath::OnEnterFarActionState()
{
	FarRange_ActionData = CurrentStageData->GetRandomFarRangeAction();

	if (!FarRange_ActionData)
	{
		ULog::Error("There are no far range actions to choose from!", true);
		FSM->PopState();
		return;
	}

	PlayActionMontage(FarRange_ActionData);

	CurrentActionType = FarRange_ActionData->ActionType;
	CurrentCounterType = FarRange_ActionData->CounterType;
	GameState->BossData.CurrentActionType = FarRange_ActionData->ActionType;
	GameState->BossData.CurrentCounterType = FarRange_ActionData->CounterType;

	StartActionLocation = CurrentLocation;

	OnBeginExecuteAction();
}

void AMordath::UpdateFarActionState(float Uptime, int32 Frames)
{
	if (FarRange_ActionData->bConstantlyFacePlayer)
		FacePlayer();
	else
		FacePlayerBasedOnActionData(FarRange_ActionData);

	// If action has finished, go back to previous state
	if (HasFinishedAction(FarRange_ActionData->ActionMontage))
		FSM->PopState();
}

void AMordath::OnExitFarActionState()
{
	// Ensure that anim montage has stopped playing when leaving this state
	StopAnimMontage();

	CurrentMontageSection = "None";
	CurrentMontageName = "None";

	CurrentActionType = ATM_None;
	CurrentCounterType = ACM_None;
	GameState->BossData.CurrentActionType = ATM_None;
	GameState->BossData.CurrentCounterType = ACM_None;

	FarRange_ActionData->bCanBeDodged = false;
	GameState->BossData.bHasAttackBegun = false;

	MID_OriginalMaterial->SetVectorParameterValue("Attack Color", OriginalAttackColor);

	OnEndExecuteAction();
}
#pragma endregion 

#pragma region Damaged
void AMordath::OnEnterDamagedState()
{
	GameState->BossData.bHasTakenDamage = true;
	AnimInstance->bIsHit = true;
}

void AMordath::UpdateDamagedState(float Uptime, int32 Frames)
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
	Super::OnEnterDeathState();

	GameState->BossData.bIsDead = true;

	GameState->BossData.OnDeath.Broadcast();

	RangeFSM->Stop();
	StageFSM->Stop();

	TimerManager->SetTimer(DeathExpiryTimerHandle, this, &AMordath::DestroySelf, DeathTime);

	OnDeath();
}

void AMordath::UpdateDeathState(const float Uptime, const int32 Frames)
{
	Super::UpdateDeathState(Uptime, Frames);
}

void AMordath::OnExitDeathState()
{
	Super::OnExitDeathState();

	GameState->BossData.bIsDead = false;
}
#pragma endregion

#pragma region Transition
void AMordath::OnEnterTransitionState()
{
	CurrentHealth = HealthComponent->GetCurrentHealth();
}

void AMordath::UpdateTransitionState(const float Uptime, int32 Frames)
{
	if (IsInThirdStage())
	{
		const float NewHealth = FMath::GetMappedRangeValueClamped({0.0f, 1.0f}, {CurrentHealth, HealthComponent->GetDefaultHealth()}, 0.5f * Uptime);
		SetHealth(NewHealth);
	}

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->PopState();
}

void AMordath::OnExitTransitionState()
{
}
#pragma endregion

#pragma region Stunned
void AMordath::OnEnterStunnedState()
{
	// Reset hit count
	HitCounter = 0;

	StopAnimMontage();
	//StopActionMontage();

	GameState->BossData.bHasTakenDamage = true;
	MordathAnimInstance->bIsStunned = true;
}

void AMordath::UpdateStunnedState(float Uptime, int32 Frames)
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

#pragma region Roar
void AMordath::OnEnterRoarState()
{
	MordathAnimInstance->bCanRoar = true;
}

void AMordath::UpdateRoarState(float Uptime, int32 Frames)
{
	if (AnimInstance->AnimTimeRemaining < 0.0f)
		FSM->Stop();
}

void AMordath::OnExitRoarState()
{
	MordathAnimInstance->bCanRoar = false;
}
#pragma endregion

#pragma region Dash
void AMordath::OnEnterDashState()
{
	DashComponent->StartCooldown();

	PlayAnimMontage(SuperCloseRange_ActionData->ActionMontage);
}

void AMordath::UpdateDashState(float Uptime, int32 Frames)
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

void AMordath::UpdateDashCombatState(float Uptime, int32 Frames)
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

#pragma region Tired
void AMordath::OnEnterTiredState()
{
	PlayActionMontage();

	TimerManager->PauseTimer(CurrentActionData->TH_ExecutionExpiry);
}

void AMordath::UpdateTiredState(float Uptime, int32 Frames)
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

#pragma region Teleport
void AMordath::OnEnterTeleportState()
{
	PlayActionMontage();

	TimerManager->PauseTimer(CurrentActionData->TH_ExecutionExpiry);

	TeleportationComponent->GenerateTeleportTime();
	TeleportationComponent->Disappear();

	SKM_Feathers->SetMaterial(0, TeleportationComponent->GetDissolveMaterial());

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionObjectType(ECC_Vehicle);

	GameState->Mordaths.Remove(this);
	GameState->Mordaths.Shrink();
	GameState->OnMordathBaseDeath.Broadcast();

	EnableInvincibility();
}

void AMordath::UpdateTeleportState(float Uptime, int32 Frames)
{
	if (Uptime >= CurrentActionMontage->SequenceLength - 0.2f)
		AnimInstance->Montage_Pause();

	if (Uptime > TeleportationComponent->GetTeleportTime() && !TeleportationComponent->IsCoolingDown())
	{
		TeleportationComponent->StartCooldown();

		float TeleportRadius;
		switch (CurrentActionData->TeleportToRange)
		{
		case BRM_Close:
			TeleportRadius = CurrentStageData->GetCloseRangeRadius();
		break;

		case BRM_Mid:
			TeleportRadius = CurrentStageData->GetMidRangeRadius();
		break;

		case BRM_Far:
			TeleportRadius = CurrentStageData->GetMidRangeRadius() * 2;
		break;

		default:
			TeleportRadius = CurrentStageData->GetMidRangeRadius();
		break;
		}

		#if !UE_BUILD_SHIPPING
		UKismetSystemLibrary::DrawDebugCircle(this, CurrentLocation * FVector(1.0f, 1.0f, 0.5f), TeleportRadius, 32, FColor::Purple, 3.0f, 5.0f, FVector::ForwardVector, FVector::RightVector);
		#endif
		
		SetActorLocation(TeleportationComponent->FindLocationToTeleport(GameState->PlayerData.Location, TeleportRadius, GameState->PlayArea));

		FSM->PopState();
	}
}

void AMordath::OnExitTeleportState()
{
	TeleportationComponent->Reappear();

	GameState->Mordaths.Add(this);

	DisableInvincibility();

	CapsuleComp->SetCollisionProfileName("Mordath");

	// Ensure that anim montage has stopped playing when leaving this state
	StopActionMontage();

	NextAction();
}
#pragma endregion  
#pragma endregion

#pragma region Boss Ranges
#pragma region Close Range
void AMordath::OnEnterCloseRange()
{
	GameState->PlayerData.CurrentRange = BRM_Close;
}

void AMordath::UpdateCloseRange(float Uptime, int32 Frames)
{
	Super::UpdateCloseRange(Uptime, Frames);
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

void AMordath::UpdateMidRange(float Uptime, int32 Frames)
{
	Super::UpdateMidRange(Uptime, Frames);
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

void AMordath::UpdateFarRange(float Uptime, int32 Frames)
{
	if (DistanceToPlayer < CurrentStageData->GetMidRangeRadius())
	{
		RangeFSM->PushState("Mid");
		return;
	}

	if (IsTeleporting() || IsSpecialAttacking() || IsPerformingFarAction() || IsPerformingAction() || IsLocked() || IsTransitioning())
		return;

	if (Uptime > CurrentStageData->Combat.FarRangeAttackDelay && !IsTired())
	{
		FSM->PopState();
		FSM->PushState("Far Action");
	}
}

void AMordath::OnExitFarRange()
{
}
#pragma endregion 

#pragma region Super Close
void AMordath::OnEnterSuperCloseRange()
{
	Super::OnEnterSuperCloseRange();

	GameState->PlayerData.CurrentRange = BRM_SuperClose;
}

void AMordath::UpdateSuperCloseRange(float Uptime, int32 Frames)
{
	if (DistanceToPlayer > CurrentStageData->GetSuperCloseRangeRadius())
	{
		RangeFSM->PopState();
		return;
	}

	#if !UE_BUILD_SHIPPING
	if (IsLocked())
		return;
	#endif

	if (IsDashing() || IsAttacking() || IsRecovering() || IsStunned() || IsKicking() || IsTired() ||
		IsPerformingCloseAction() || IsPerformingFarAction() || IsTeleporting() || IsTransitioning())
		return;

	if (Uptime > CurrentStageData->GetSuperCloseRangeTime())
	{
		const bool bWantsKick = FMath::RandBool();
		if (bWantsKick && (IsInSecondStage() || IsInThirdStage()))
		{
			FSM->PopState();
			FSM->PushState("Kick");
		}
		else
		{	
			FSM->PopState();
			FSM->PushState("Close Action");
		}
	}
}

void AMordath::OnExitSuperCloseRange()
{
	Super::OnExitSuperCloseRange();
}
#pragma endregion 
#pragma endregion

#pragma region Boss Stages
#pragma region Stage 1
void AMordath::OnEnterFirstStage()
{
	CurrentStageData = GetStageData();
	CurrentStageData->InitStageData();

	SuperCloseRange_ActionData = CurrentStageData->GetRandomSuperCloseRangeAction();

	MordathAnimInstance->CurrentStage = Stage_1;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[0];

	BeginFirstStage.Broadcast();
}

void AMordath::UpdateFirstStage(float Uptime, int32 Frames)
{
	// Can we enter the second stage?
	if (HealthComponent->GetCurrentHealth() <= HealthComponent->GetDefaultHealth() * SecondStageHealth)
	{
		GameState->BossData.OnEnterSecondStage.Broadcast();
		return;
	}

	if (IsLocked())
		return;

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

void AMordath::OnExitFirstStage()
{

}
#pragma endregion 

#pragma region Stage 2
void AMordath::OnEnterSecondStage()
{
	CurrentStageData = GetStageData();

	if (Stage2_Transition)
		PlayAnimMontage(Stage2_Transition);

	SuperCloseRange_ActionData = CurrentStageData->GetRandomSuperCloseRangeAction();

	MordathAnimInstance->CurrentStage = Stage_2;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];

	BeginSecondStage.Broadcast();
}

void AMordath::UpdateSecondStage(float Uptime, int32 Frames)
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

	if (IsLocked() || IsTeleporting())
		return;

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
		// Decide which attack to choose
		if (!IsWaitingForNextCombo() && !IsDelayingAction())
			ChooseAction();
	}
}

void AMordath::OnExitSecondStage()
{

}
#pragma endregion 

#pragma region Stage 3
void AMordath::OnEnterThirdStage()
{
	CurrentStageData = GetStageData();

	if (Stage3_Transition)
		PlayAnimMontage(Stage3_Transition);

	SuperCloseRange_ActionData = CurrentStageData->GetRandomSuperCloseRangeAction();

	MordathAnimInstance->CurrentStage = Stage_3;
	MordathAnimInstance->ActiveStateMachine = MordathAnimInstance->StateMachines[1];

	PlayerController->ClientPlayCameraShake(CurrentStageData->GetRoarShake().Shake, CurrentStageData->GetRoarShake().Intensity);

	EnergyShieldCollision->SetSphereRadius(CurrentStageData->GetSuperCloseRangeRadius());

	BeginThirdStage.Broadcast();
}

void AMordath::UpdateThirdStage(float Uptime, int32 Frames)
{
	if (AnimInstance->Montage_IsPlaying(Stage3_Transition))
		return;

//#if !UE_BUILD_SHIPPING
//	// Can we enter the second stage?
//	if (HealthComponent->GetCurrentHealth() > HealthComponent->GetDefaultHealth() * ThirdStageHealth)
//	{
//		GameState->BossData.OnEnterSecondStage.Broadcast();
//		return;
//	}
//#endif

	if (GameState->Mordaths.Num() > MaxGhosts && !IsInvincible())
	{
		FSM->PushState("Invincible");
		return;
	}

	if (IsLocked() || IsTeleporting() || IsInvincible() || IsTransitioning())
		return;

	if (ChosenCombo->IsAtLastAction() && !IsWaitingForNextCombo())
	{
		if (CurrentStageData->ComboSettings.bDelayBetweenCombo)
			ChooseComboDelayed();
		else
			ChooseCombo();

		return;
	}

	if (CanAttack() && !IsTeleporting())
	{
		// Decide which attack to choose
		if (!IsWaitingForNextCombo() && !IsDelayingAction())
			ChooseAction();
	}
}

void AMordath::OnExitThirdStage()
{
}
#pragma endregion   
#pragma endregion

#pragma region Events
void AMordath::OnFullHealth()
{
	if (IsInThirdStage())
	{
		HealthComponent->SetDefaultHealth(ThirdStageDefaultHealth);
		UpdateCharacterInfo();
	}
}

void AMordath::OnAttackEnd_Implementation(UAnimMontage* Montage, const bool bInterrupted)
{
	if (GameState->IsPlayerDead())
	{
		FSM->PushState("Roar");
	}
}

void AMordath::OnLowHealth()
{
}

void AMordath::OnPlayerDeath()
{
	Super::OnPlayerDeath();

	RangeFSM->Stop();
	StageFSM->Stop();
}

void AMordath::OnAttackParryed()
{
	if (IsInvincible())
		return;

	if ((CurrentActionData->Action->CounterType == ACM_Parryable || CurrentActionData->Action->CounterType == ACM_ParryableBlockable)  && !IsStunned())
	{
		PreviousActionMontage = AnimInstance->GetCurrentActiveMontage();

		if (PreviousActionMontage)
			PreviousActionMontage->BlendOut.SetBlendTime(CurrentStageData->GetAttackBlockedBlendOutTime());

		StopActionMontage();

		FSM->PopState();
		FSM->PushState("Stunned");

		// Shake the camera
		GameState->CurrentCameraShake = GameState->GetCameraManager()->PlayCameraShake(CurrentStageData->GetStunShake().Shake, CurrentStageData->GetStunShake().Intensity);
	}
}

void AMordath::OnAttackBlocked()
{
	if (IsInvincible())
		return;

	if ((CurrentActionData->Action->CounterType == ACM_Blockable || CurrentActionData->Action->CounterType == ACM_ParryableBlockable) && !IsDamaged())
	{
		PreviousActionMontage = AnimInstance->GetCurrentActiveMontage();

		if (PreviousActionMontage)
			PreviousActionMontage->BlendOut.SetBlendTime(0.0f);

		StopActionMontage();

		FSM->PopState();
		FSM->PushState("Damaged");
		
		// Shake the camera
		GameState->CurrentCameraShake = GameState->GetCameraManager()->PlayCameraShake(CurrentStageData->GetDamagedShake().Shake, CurrentStageData->GetDamagedShake().Intensity);
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

	StopMovement();

	FSM->RemoveAllStates();
	FSM->PushState("Transition");

	StageFSM->PushState(1);
	StageFSM->PopState(0);

	CachedCombos.Empty();
	ChooseCombo();
}

void AMordath::OnThirdStageHealth()
{
	const FVector NewLocation = CurrentLocation * FVector(1.0f, 1.0f, 0.0f);
	SpawnLightningStrike(NewLocation);

	StopMovement();

	SpawnGhost();

	FSM->RemoveAllStates();
	FSM->PushState("Transition");

	StageFSM->PushState(2);
	StageFSM->PopState(1);
	StageFSM->PopState(0);

	CachedCombos.Empty();
	ChooseCombo();
}

void AMordath::OnDisappeared()
{
	GameState->BossData.OnMordathDisappeared.Broadcast();
}

void AMordath::OnReappeared()
{
	GameState->BossData.OnMordathReappeared.Broadcast();

	//MID_OriginalMaterial = SKMComponent->CreateDynamicMaterialInstance(0, OriginalMaterial, FName("MID_Mordath"));
	//SKMComponent->SetMaterial(0, MID_OriginalMaterial);
	//SKM_Feathers->SetMaterial(0, MID_OriginalMaterial);

	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	DisableInvincibility();
}

void AMordath::OnBeginDisappear()
{
	GameState->BossData.OnMordathBeginDisappear.Broadcast();
}

void AMordath::OnBeginReappear()
{
	GameState->BossData.OnMordathBeginReappear.Broadcast();

	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	DisableInvincibility();
}

void AMordath::OnEnterPerfectDashWindow()
{
	SpawnPerfectDashEmitter();

	//if (CurrentCounterType == ACM_Parryable || CurrentCounterType == ACM_ParryableBlockable)
	//	FlashIndicator->Flash(FlashIndicator->ParryableFlashColor);
}

void AMordath::OnEnterEnergySphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AMordathBase::StaticClass()) && OtherActor->IsA(ACharacter::StaticClass()))
	{
		bHasActorEnteredEnergySphere = true;
		CharacterInEnergySphere = Cast<ACharacter>(OtherActor);

		if (IsInvincible())
			CharacterInEnergySphere->TakeDamage(10.0f, FDamageEvent(UDmgType_MordathElectricShield::StaticClass()), Controller, this);
			GameState->BossData.OnActorEnterEnergySphere.Broadcast();
	}
}

void AMordath::OnExitEnergySphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->IsA(AMordathBase::StaticClass()) && OtherActor->IsA(ACharacter::StaticClass()))
	{
		bHasActorEnteredEnergySphere = false;
		CharacterInEnergySphere = nullptr;
		GameState->BossData.OnActorExitEnergySphere.Broadcast();
	}
}
#pragma endregion

void AMordath::DestroySelf()
{
	FSMVisualizer->UnhighlightState(FSM->GetActiveStateName().ToString());
	FSMVisualizer->UnhighlightState(RangeFSM->GetActiveStateName().ToString());
	FSMVisualizer->UnhighlightState(StageFSM->GetActiveStateName().ToString());

	Destroy();
}

void AMordath::StopActionMontage()
{
	Super::StopActionMontage();

	CurrentActionData->bExecutionTimeExpired = false;

	CurrentMontageSection = "None";
	CurrentMontageName = "None";

	CurrentActionData->Action->bCanBeDodged = false;

	CurrentActionType = ATM_None;
	CurrentCounterType = ACM_None;
	GameState->BossData.CurrentActionType = ATM_None;
	GameState->BossData.CurrentCounterType = ACM_None;
	GameState->BossData.bHasAttackBegun = false;
}

void AMordath::UpdateCharacterInfo()
{
	GameState->BossData.StartingHealth = HealthComponent->GetDefaultHealth();

	GameState->BossData.Health = HealthComponent->GetCurrentHealth();
	GameState->BossData.SmoothedHealth = HealthComponent->GetSmoothedHealth();
}

void AMordath::BroadcastLowHealth()
{
	GameState->BossData.OnLowHealth.Broadcast();
	bWasLowHealthEventTriggered = true;
}

void AMordath::BeginTakeDamage(const float DamageAmount, const FDamageEvent& DamageEvent)
{
	Super::BeginTakeDamage(DamageAmount, DamageEvent);

	// Shake the camera
	GameState->CurrentCameraShake = GameState->GetCameraManager()->PlayCameraShake(CurrentStageData->GetDamagedShake().Shake, CurrentStageData->GetDamagedShake().Intensity);
}

void AMordath::ApplyDamage(const float DamageAmount, const FDamageEvent& DamageEvent)
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

	if (GameState->PlayerData.CurrentAttackType == ATP_Charge || GameState->PlayerData.CurrentAttackType == ATP_Dash)
	{
		#if !UE_BUILD_SHIPPING
		if (IsLocked())
			return;
		#endif

		FSM->PopState();
		FSM->PushState("Stunned");
	}

	if (GameState->PlayerData.CurrentAttackType == ATP_Charge)
		OnLifeStolen();
}

void AMordath::EndTakeDamage(const FDamageEvent& DamageEvent)
{
	// Handled in blueprints
	if (static_cast<int32>(RecentDamage) > 0)
		OnAfterTakeDamage();

	// Are we dead?
	if (HealthComponent->GetCurrentHealth() <= 0.0f && !bIsDead && IsInThirdStage())
	{
		Die();
	}
}

void AMordath::LockSelf()
{
	bIsLocked = true;

	FSM->PushState("Locked");
}

void AMordath::UnlockSelf()
{
	bIsLocked = false;

	FSM->PopState("Locked");
}

void AMordath::ToggleLockSelf()
{
	bIsLocked = !bIsLocked;

	if (bIsLocked)
		FSM->PushState("Locked");
	else
		FSM->PopState("Locked");
}

void AMordath::ChooseCombo()
{
	Super::ChooseCombo();

	FString NewMontageName = CurrentActionMontage->GetName();
	NewMontageName.RemoveAt(0, 11);
	CurrentMontageName = NewMontageName;

	#if !UE_BUILD_SHIPPING
	if (Debug.bLogCurrentStageCombo)
	{
		switch (StageFSM->GetActiveStateID())
		{
		case 0:
			ULog::Info("Using stage 1 combos", true);
		break;

		case 1:
			ULog::Info("Using stage 2 combos", true);
		break;

		case 2:
			ULog::Info("Using stage 3 combos", true);
		break;

		default:
		break;
		}
	}
	#endif
}

float AMordath::ChooseComboDelayed()
{
	#if !UE_BUILD_SHIPPING
		const float NewDelayTime = Super::ChooseComboDelayed();

		if (Debug.bLogComboDelayTime)
			ULog::DebugMessage(INFO, "Delaying: " + FString::SanitizeFloat(NewDelayTime) + " before next combo", true);

		return NewDelayTime;
	#else
		return Super::ChooseComboDelayed();
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

	CurrentActionMontage = ActionDataToUse->ActionMontage;

	FString NewMontageName = CurrentActionMontage->GetName();
	NewMontageName.RemoveAt(0, 11);
	CurrentMontageName = NewMontageName;

	// Do a flash to indicate what kind of attack this is
	switch (ActionDataToUse->CounterType)
	{
	case ACM_Parryable:
		MID_OriginalMaterial->SetVectorParameterValue("Attack Color", FColor::Yellow);
		//FlashIndicator->Flash(FlashIndicator->ParryableFlashColor);
	break;
	
	case ACM_Blockable:
		MID_OriginalMaterial->SetVectorParameterValue("Attack Color", FColor::White);
		//FlashIndicator->Flash(FlashIndicator->BlockableFlashColor);
	break;
	
	case ACM_ParryableBlockable:
		MID_OriginalMaterial->SetVectorParameterValue("Attack Color", FColor::Yellow);
		//FlashIndicator->Flash(FlashIndicator->ParryableFlashColor);
	break;
	
	case ACM_NoCounter:
		MID_OriginalMaterial->SetVectorParameterValue("Attack Color", FColor::Red);
		//FlashIndicator->Flash(FlashIndicator->NoCounterFlashColor);
	break;
	
	default:
	break;
	}

	if (CurrentActionData->bSpawnGhost)
	{
		if (CurrentActionData->GhostCount > 1)
			SpawnGhostDelayed(CurrentActionData->GhostCount);
		else
			SpawnGhost();
	}

	// Update data
	CurrentActionType = ActionDataToUse->ActionType;
	CurrentCounterType = ActionDataToUse->CounterType;
	ActionDamage = ActionDataToUse->ActionDamage;
	GameState->BossData.CurrentActionType = ActionDataToUse->ActionType;
	GameState->BossData.CurrentCounterType = ActionDataToUse->CounterType;

	ExecuteAction(ActionDataToUse);
}

void AMordath::UpdateDamageValueInMainHUD(const float DamageAmount) const
{
	MainHUD->FadeInDamageValue();
	MainHUD->UpdateDamageValue(DamageAmount);
}

void AMordath::ExecuteAction(UMordathActionData* ActionData)
{
	Super::ExecuteAction(ActionData);

	switch (ActionData->ActionType)
	{
	case ATM_Teleport:
		FSM->PushState("Teleport");
	break;

	case ATM_Tired:
		FSM->PushState("Tired");
	break;

	default:
		FSM->PushState("Action");
	break;
	}
}

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// We don't want to be damaged when we're already dead
	if (bIsDead || AnimInstance->bIsHit || DamageCauser->IsA(AMordathGhost::StaticClass()) || IsTransitioning())
		return DamageAmount;

	BeginTakeDamage(DamageAmount, DamageEvent);

	// Apply damage once
	if (!AnimInstance->bIsHit)
	{
		ApplyDamage(DamageAmount, DamageEvent);
	}

	EndTakeDamage(DamageEvent);

	return DamageAmount;
}

void AMordath::ChangeHitboxSize(const float NewRadius)
{
}

bool AMordath::IsStunned() const
{
	return FSM->GetActiveStateID() == 14;
}

bool AMordath::IsKicking() const
{
	return FSM->GetActiveStateID() == 20;
}

bool AMordath::CanAttack() const
{
	return (CurrentActionData->RangeToExecute == RangeFSM->GetActiveStateID() || 
			CurrentActionData->RangeToExecute == BRM_AnyRange || 
			IsExecutionTimeExpired() || 
			ChosenCombo->WantsToExecuteNonStop()) &&
			!IsRecovering() && !IsAttacking() && !IsDashing() && !IsTransitioning() && !IsStunned() && !IsDamaged() && !IsStrafing() && !IsTired() && !IsPerformingCloseAction() && !IsPerformingFarAction() && !IsLocked() && !IsTeleporting() && !IsInvincible();
}

void AMordath::Die()
{
	Super::Die();

	FSM->RemoveAllStates();
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

bool AMordath::IsDashing() const
{
	return FSM->GetActiveStateID() == 16 || FSM->GetActiveStateID() == 17 || IsPerformingAction() && (CurrentActionData->Action->ActionType == ATM_Dash_Backward || CurrentActionData->Action->ActionType == ATM_Dash_Forward || CurrentActionData->Action->ActionType == ATM_Dash_Left || CurrentActionData->Action->ActionType == ATM_Dash_Right);
}

bool AMordath::IsStrafing() const
{
	return IsPerformingAction() && (CurrentActionType == ATM_Strafe_Left || CurrentActionType == ATM_Strafe_Right);
}

bool AMordath::IsDamaged() const
{
	return FSM->GetActiveStateID() == 12;
}

bool AMordath::IsRecovering() const
{
	return FSM->GetActiveStateID() == 21;
}

bool AMordath::IsPerformingCloseAction() const
{
	return FSM->GetActiveStateID() == 26;
}

bool AMordath::IsTransitioning() const
{
	return FSM->GetActiveStateID() == 13;
}

bool AMordath::IsTired() const
{
	return FSM->GetActiveStateID() == 23;
}

bool AMordath::IsTeleporting() const
{
	return FSM->GetActiveStateID() == 18;
}

bool AMordath::IsPerformingFarAction() const
{
	return FSM->GetActiveStateID() == 27;
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

	default:
	break;
	}
}

bool AMordath::IsInvincible() const
{
	return FSM->GetActiveStateID() == 28;
}

void AMordath::SpawnGhost()
{
	if (GameState->Mordaths.Num() > MaxGhosts)
	{
		SKM_ElectricShield->SetVisibility(true);

		StopMovement();

		FSM->RemoveAllStates();
		FSM->PushState("Tired");
		return;
	}

	FVector NewLocation;
	if (GameState->BossData.GhostSpawnPoints.Num() > 2)
	{
		const uint16 RandomIndex = FMath::RandRange(0, GameState->BossData.GhostSpawnPoints.Num() - 1);
		NewLocation = GameState->BossData.GhostSpawnPoints[RandomIndex];
	}
	else
	{
		const uint16 RandomIndex = FMath::RandRange(0, GameState->PlayArea->GetNodes().Num() - 1);
		NewLocation = GameState->PlayArea->GetNodes()[RandomIndex];
	}

	NewLocation.Z = CurrentLocation.Z;

	//const auto Transform = GetActorTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor(GhostClass, &NewLocation, &CurrentRotation, SpawnParameters);
}

void AMordath::SpawnGhostDelayed(const int32 Amount, const float DelayInterval)
{
	if (GameState->Mordaths.Num() > MaxGhosts)
		return;

	static int32 CurrentAmount = 0;
	if (CurrentAmount >= Amount)
	{
		CurrentAmount = 0;
		TimerManager->ClearTimer(TH_SpawnGhostDelay);
		return;
	}

	SpawnGhost();

	FTimerDelegate TD_SpawnGhostDelay;
	TD_SpawnGhostDelay.BindUFunction(this, "SpawnGhostDelayed", Amount, DelayInterval);

	TimerManager->SetTimer(TH_SpawnGhostDelay, TD_SpawnGhostDelay, DelayInterval, true);

	CurrentAmount++;
}

float AMordath::GetActionDamage() const
{
	switch (GameInstance->ChosenDifficultyOption)
	{
	case DO_Casual:
		return ActionDamage * CasualDifficulty->DamageMultiplier;

	case DO_Experienced:
		return ActionDamage * ExperiencedDifficulty->DamageMultiplier;

	case DO_Realistic:
		return ActionDamage * RealisticDifficulty->DamageMultiplier;

	default:
		return ActionDamage;
	}
}

void AMordath::SpawnLightningStrike(const FVector& LocationToSpawn, const FRotator& Rotation)
{
	if (LightningStrikeClass)
		World->SpawnActor(LightningStrikeClass, &LocationToSpawn, &Rotation);
	else
		ULog::Error("Lightning Strike class is null", true);
}

USkeletalMeshComponent* AMordath::GetShield()
{
	for (auto Component : Components)
	{
		if (Component->GetName() == "EnergyShieldMesh")
			return Cast<USkeletalMeshComponent>(Component);
	}

	#if !UE_BUILD_SHIPPING
	ULog::Error("Could not find shield for " + GetName(), true);
	#endif

	return nullptr;
}

UMordathDifficultyData* AMordath::GetDifficultyData() const
{
	switch (GameInstance->ChosenDifficultyOption)
	{
	case DO_Casual:
		return CasualDifficulty;

	case DO_Experienced:
		return ExperiencedDifficulty;

	case DO_Realistic:
		return RealisticDifficulty;

	default:
		return CasualDifficulty;
	}
}

UMordathStageData* AMordath::GetStageData() const
{
	switch (StageFSM->GetActiveStateID())
	{
	case 0: /*Stage 1*/
		return CurrentDifficultyData->StageOneData;

	case 1: /*Stage 2*/
		return CurrentDifficultyData->StageTwoData;

	case 2: /*Stage 3*/
		return CurrentDifficultyData->StageThreeData;

	default:
		return nullptr;
	}
}

void AMordath::AddDebugMessages()
{
	const float YPadding = 5.0f;

	OverthroneHUD->AddOnScreenDebugMessage("Boss", FColor::White, YPadding, 1.1f);
	OverthroneHUD->AddOnScreenDebugMessage("Boss Health: ", FColor::Green, YPadding);
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
	OverthroneHUD->AddOnScreenDebugMessage("Current Action Montage: ", FColor::Yellow, YPadding);
}
