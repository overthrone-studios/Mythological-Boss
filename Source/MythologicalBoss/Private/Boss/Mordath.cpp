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
#include "ApexDestruction/Public/DestructibleComponent.h"
#include "ApexDestruction/Public/DestructibleActor.h"
#include "FSM.h"
#include "Log.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

AMordath::AMordath()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Boss/Animations/AnimBP_Boss.AnimBP_Boss_C'"));

	// Get the hit animation to use when being hit
	HitAnimation = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), nullptr, TEXT("AnimSequence'/Game/Characters/Boss/Animations/Boss_Damaged.Boss_Damaged'")));

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
			ULog::LogDebugMessage(ERROR, FString("AnimBP did not succeed."));
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

	BossStateMachine->GetState(12)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDamagedState);
	BossStateMachine->GetState(12)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDamagedState);
	BossStateMachine->GetState(12)->OnExitState.AddDynamic(this, &AMordath::OnExitDamagedState);

	BossStateMachine->GetState(13)->OnEnterState.AddDynamic(this, &AMordath::OnEnterDeathState);
	BossStateMachine->GetState(13)->OnUpdateState.AddDynamic(this, &AMordath::UpdateDeathState);
	BossStateMachine->GetState(13)->OnExitState.AddDynamic(this, &AMordath::OnExitDeathState);

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
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	// Cache the world object
	World = GetWorld();

	// Cache the movement component
	MovementComponent = GetCharacterMovement();

	Health = StartingHealth;

	// Cache the Overthrone HUD
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	PlayerHUD = Cast<UMainPlayerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("MainHUD"));

	// Cache our anim instance
	AnimInstance = Cast<UMordathAnimInstance>(GetMesh()->GetAnimInstance());

	// Cache our game instance
	GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->BossStartingHealth = StartingHealth;
	SendInfo();

	// Cache the FSM Visualizer HUD
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("BossFSMVisualizer"));

	BossStateMachine->InitState(0);

	GetWorld()->GetTimerManager().SetTimer(UpdateInfoTimerHandle, this, &AMordath::SendInfo, 0.05f, true);
}

void AMordath::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AMordath::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);

	Super::PossessedBy(NewController);
}

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health > 0.0f && !AnimInstance->bIsHit)
	{
		if (BossStateMachine->GetActiveStateName() != "Idle")
			BossStateMachine->PopState();

		BossStateMachine->PushState("Damaged");

		Health -= DamageAmount;
		ULog::LogDebugMessage(INFO, FString::SanitizeFloat(DamageAmount) + FString(" damaged received"), true);
	}

	if (Health <= 0.0f)
	{
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

void AMordath::OnEnterIdleState()
{
	if (!GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		BossStateMachine->PushState("Follow");
}

void AMordath::UpdateIdleState()
{
	ULog::LogDebugMessage(INFO, "Idle state", true);

	if (GetDistanceToPlayer() > 200.0f)
		BossStateMachine->PushState("Follow");
}

void AMordath::OnExitIdleState()
{
}

void AMordath::OnEnterFollowState()
{
	AnimInstance->bIsWalking = true;
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

	switch (BossAIController->MoveToActor(PlayerCharacter, 200.0f))
	{
	case EPathFollowingRequestResult::RequestSuccessful:
		if (GetDistanceToPlayer() <= 400.0f)
		{
			ChooseCombo();

			BossStateMachine->PushState(3);
		}
		else if (GetVelocity().IsZero() && MovementComponent->IsMovingOnGround())
		{
			BossStateMachine->PopState();
		}
		break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		const FRotator TargetRotation = FacePlayer();

		if (GetActorRotation().Equals(TargetRotation, 50.0f))
			BossStateMachine->PushState(3);
	}
	break;

	default:
		break;
	}
}

void AMordath::OnExitFollowState()
{
	AnimInstance->bIsWalking = false;
}

void AMordath::OnEnterLightAttack1State()
{
	AnimInstance->bAcceptLightAttack = true;
}

void AMordath::UpdateLightAttack1State()
{
	// If attack animation has finished, go back to previous state
	const int32 StateIndex = AnimInstance->GetStateMachineInstance(AnimInstance->GenericsMachineIndex)->GetCurrentState();
	const float TimeRemaining = AnimInstance->GetRelevantAnimTimeRemaining(AnimInstance->GenericsMachineIndex, StateIndex);

	if (TimeRemaining <= 0.2f)
		BossStateMachine->PopState();
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

}

void AMordath::OnExitLightAttack3State()
{
	AnimInstance->bAcceptThirdLightAttack = false;
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

	if (TimeRemaining <= 0.1f)
		BossStateMachine->PopState();
}

void AMordath::OnExitDamagedState()
{
	AnimInstance->bIsHit = false;
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

float AMordath::GetDistanceToPlayer() const
{
	return FVector::Dist(GetActorLocation(), UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation());
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

}
