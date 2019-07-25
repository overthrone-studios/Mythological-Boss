// Copyright Overthrone Studios 2019

#include "Mordath.h"
#include "Public/OverthroneHUD.h"
#include "Public/OverthroneGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
			ULog::LogDebugMessage(ERROR, FString("AnimBP did not succeed."));
	}

	// Create a FSM
	BossStateMachine = CreateDefaultSubobject<UFSM>(FName("Boss FSM"));
	BossStateMachine->AddState(0, "Idle");
	BossStateMachine->AddState(1, "Walk");
	BossStateMachine->AddState(2, "Run");
	BossStateMachine->AddState(3, "Light Attack 1");
	BossStateMachine->AddState(4, "Light Attack 2");
	BossStateMachine->AddState(5, "Light Attack 3");
	BossStateMachine->AddState(6, "Heavy Attack 1");
	BossStateMachine->AddState(7, "Heavy Attack 2");
	BossStateMachine->AddState(8, "Heavy Attack 3");
	BossStateMachine->AddState(9, "Special Attack 1");
	BossStateMachine->AddState(10, "Special Attack 2");
	BossStateMachine->AddState(11, "Special Attack 3");

	// Bind state events to our functions
	BossStateMachine->GetState(0)->OnEnterState.AddDynamic(this, &AMordath::OnEnterIdleState);
	BossStateMachine->GetState(0)->OnUpdateState.AddDynamic(this, &AMordath::UpdateIdleState);
	BossStateMachine->GetState(0)->OnExitState.AddDynamic(this, &AMordath::OnExitIdleState);

	BossStateMachine->GetState(1)->OnEnterState.AddDynamic(this, &AMordath::OnEnterWalkState);
	BossStateMachine->GetState(1)->OnUpdateState.AddDynamic(this, &AMordath::UpdateWalkState);
	BossStateMachine->GetState(1)->OnExitState.AddDynamic(this, &AMordath::OnExitWalkState);

	BossStateMachine->GetState(2)->OnEnterState.AddDynamic(this, &AMordath::OnEnterRunState);
	BossStateMachine->GetState(2)->OnUpdateState.AddDynamic(this, &AMordath::UpdateRunState);
	BossStateMachine->GetState(2)->OnExitState.AddDynamic(this, &AMordath::OnExitRunState);

	BossStateMachine->GetState(3)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack1State);
	BossStateMachine->GetState(3)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack1State);
	BossStateMachine->GetState(3)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack1State);

	BossStateMachine->GetState(4)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack2State);
	BossStateMachine->GetState(4)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack2State);
	BossStateMachine->GetState(4)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack2State);

	BossStateMachine->GetState(5)->OnEnterState.AddDynamic(this, &AMordath::OnEnterLightAttack3State);
	BossStateMachine->GetState(5)->OnUpdateState.AddDynamic(this, &AMordath::UpdateLightAttack3State);
	BossStateMachine->GetState(5)->OnExitState.AddDynamic(this, &AMordath::OnExitLightAttack3State);

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
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void AMordath::BeginPlay()
{
	Super::BeginPlay();

	// Cache the world object
	World = GetWorld();

	// Cache the movement component
	MovementComponent = GetCharacterMovement();

	// Cache the player HUD
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	OverthroneHUD->Init();

	// Cache our game instance
	GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));

	// Cache the FSM Visualizer HUD
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD("BossFSMVisualizer"));

	BossStateMachine->InitState(0);

	GetWorld()->GetTimerManager().SetTimer(UpdateLocationTimerHandle, this, &AMordath::SendLocation, 0.05f, true);
}

void AMordath::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AMordath::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!GetMesh()->IsPlaying())
	{
		//Mesh->PlayAnimation()
		
		ULog::LogDebugMessage(INFO, FString::SanitizeFloat(DamageAmount) + FString(" damaged received"));
	}

	return DamageAmount;
}

void AMordath::SendLocation()
{
	GameInstance->BossLocation = GetActorLocation();
}

void AMordath::OnEnterIdleState()
{
	//FSMVisualizer->HighlightState(BossStateMachine->GetActiveStateName().ToString());
}

void AMordath::UpdateIdleState()
{
	//FSMVisualizer->UpdateStateUptime(BossStateMachine->GetActiveStateName().ToString(), BossStateMachine->GetActiveStateUptime());
}

void AMordath::OnExitIdleState()
{
	//FSMVisualizer->UnhighlightState(BossStateMachine->GetActiveStateName().ToString());
}

void AMordath::OnEnterWalkState()
{
}

void AMordath::UpdateWalkState()
{
}

void AMordath::OnExitWalkState()
{
}

void AMordath::OnEnterRunState()
{
}

void AMordath::UpdateRunState()
{
}

void AMordath::OnExitRunState()
{
}

void AMordath::OnEnterLightAttack1State()
{
}

void AMordath::UpdateLightAttack1State()
{
}

void AMordath::OnExitLightAttack1State()
{
}

void AMordath::OnEnterLightAttack2State()
{
}

void AMordath::UpdateLightAttack2State()
{
}

void AMordath::OnExitLightAttack2State()
{
}

void AMordath::OnEnterLightAttack3State()
{
}

void AMordath::UpdateLightAttack3State()
{
}

void AMordath::OnExitLightAttack3State()
{
}

