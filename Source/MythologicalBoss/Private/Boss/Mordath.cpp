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

	// Configure character settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossAIController::StaticClass();
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	bCanBeDamaged = true;
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	bCanBeDamaged = true;

	// Cache the world object
	World = GetWorld();

	// Cache the movement component
	MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = WalkSpeed;

	Health = StartingHealth;

	CachedCombos = Combos;

	// Cache the Overthrone HUD
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	PlayerHUD = Cast<UMainPlayerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("MainHUD"));

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
}

void AMordath::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);

	Super::PossessedBy(NewController);
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

void AMordath::EnableInvincibility()
{
	bCanBeDamaged = false;
}

void AMordath::DisableInvincibility()
{
	bCanBeDamaged = true;
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

void AMordath::OnEnterFollowState()
{
	MovementComponent->SetMovementMode(MOVE_Walking);

	if (ChosenCombo->IsAtLastAttack() && !GetWorldTimerManager().IsTimerActive(ComboDelayTimerHandle))
	{
		if (bDelayBetweenCombo)
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

	switch (BossAIController->MoveToActor(PlayerCharacter, AcceptanceRadius))
	{
	case EPathFollowingRequestResult::RequestSuccessful:

		// If we are in range
		if (GetDistanceToPlayer() <= 400.0f && bCanAttack)
		{
			ChooseAttack();
		}
		
		if (GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		{
			BossStateMachine->PopState();
		}
	break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
		if (bCanAttack)
			ChooseAttack();
	break;

	default:
	break;
	}
}

void AMordath::OnExitFollowState()
{
}

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

void AMordath::OnEnterHeavyAttack2State()
{
	AnimInstance->bAcceptSecondHeavyAttack = true;
}

void AMordath::UpdateHeavyAttack2State()
{
	FacePlayer();

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
}

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

void AMordath::OnEnterDeathState()
{
	AnimInstance->bIsDead = true;

	BossAIController->StopBT();

	GameInstance->OnBossDeath.Broadcast();
}

void AMordath::UpdateDeathState()
{
	// If death animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.1f)
	{
		BossStateMachine->PopState();

		Destroy();
	}
}

void AMordath::OnExitDeathState()
{
	AnimInstance->bIsDead = false;
}

void AMordath::OnEnterStunnedState()
{
	AnimInstance->bIsStunned = true;

	GetWorldTimerManager().SetTimer(StunExpiryTimerHandle, this, &AMordath::FinishStun, StunDuration);
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

float AMordath::GetDistanceToPlayer() const
{
	const float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());
	//ULog::DebugMessage(INFO, FString("Distance: ") + FString::SanitizeFloat(Distance), true);
	return Distance;
}

void AMordath::OnPlayerDeath()
{
	MovementComponent->DisableMovement();

	BossStateMachine->RemoveAllStatesFromStack();

	BossStateMachine->PushState("Laugh");
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
	if (bChooseRandomCombo)
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

		bCanAttack = true;
		MovementComponent->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		ComboIndex = 0;

		CachedCombos = Combos;
		ChooseCombo();
	}
}

void AMordath::FinishStun()
{
	BossStateMachine->PopState();
}

void AMordath::ChooseComboWithDelay()
{
	bCanAttack = false;

	if (RandomDeviation == 0.0f)
	{
		GetWorldTimerManager().SetTimer(ComboDelayTimerHandle, this, &AMordath::ChooseCombo, ComboDelayTime);
		return;
	}

	const float Min = ComboDelayTime - RandomDeviation;
	const float Max = ComboDelayTime + RandomDeviation;
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
