// Copyright Overthrone Studios 2019

#include "MordathBase.h"

#include "OverthroneGameState.h"

#include "BossAIController.h"
#include "MordathAnimInstance.h"

#include "Combat/MordathActionData.h"
#include "Misc/MordathStageData.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "FSM.h"

#include "ConstructorHelpers.h"
#include "OverthroneFunctionLibrary.h"

AMordathBase::AMordathBase() : AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Get our anim blueprint class
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Characters/Mordath/Animations/ABP_Mordath.ABP_Mordath_C'"));

	// Get the skeletal mesh to use
	SkeletalMesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, TEXT("SkeletalMesh'/Game/Characters/Mordath/SKM_Mordath.SKM_Mordath'")));

	// Configure our mesh
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -140.0f));
		GetMesh()->SetWorldScale3D(FVector(1.3f));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		if (AnimBP.Succeeded())
			GetMesh()->AnimClass = AnimBP.Class;
	}

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

void AMordathBase::BeginPlay()
{
	Super::BeginPlay();
	
	MordathAnimInstance = Cast<UMordathAnimInstance>(SKMComponent->GetAnimInstance());
	PlayerCharacter = UOverthroneFunctionLibrary::GetPlayerCharacter(this);
}

void AMordathBase::Tick(const float DeltaTime)
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
	
	DistanceToPlayer = GetDistanceToPlayer();
	DirectionToPlayer = GetDirectionToPlayer();

	AnimInstance->MovementSpeed = CurrentMovementSpeed;
	AnimInstance->ForwardInput = ForwardInput;
	AnimInstance->RightInput = RightInput;
}

void AMordathBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BossAIController = Cast<ABossAIController>(NewController);
}

void AMordathBase::Die()
{
	Super::Die();

	MordathAnimInstance->LeaveAllStates();

	BossAIController->StopMovement();
	MovementComponent->SetMovementMode(MOVE_None);
	MovementComponent->DisableMovement();

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FSM->RemoveAllStates();
}

void AMordathBase::FacePlayer(const float RotationSpeed)
{
	if (RotationSpeed > 0.0f)
		SetActorRotation(FMath::Lerp(ControlRotation, FRotator(ControlRotation.Pitch, DirectionToPlayer.Rotation().Yaw, ControlRotation.Roll), RotationSpeed * World->DeltaTimeSeconds));
}

void AMordathBase::FacePlayer_Instant()
{
	SetActorRotation(FRotator(ControlRotation.Pitch, DirectionToPlayer.Rotation().Yaw, ControlRotation.Roll));
}

void AMordathBase::MoveForward(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * DirectionToPlayer);
	ForwardInput = Scale;
	RightInput = 0.0f;
}

void AMordathBase::MoveRight(float Scale)
{
	Scale = FMath::Clamp(Scale, -1.0f, 1.0f);

	AddMovementInput(Scale * GetActorRightVector());
	ForwardInput = 0.0f;
	RightInput = Scale;
}

void AMordathBase::ChooseMovementDirection()
{
	MovementDirection = FMath::RandRange(0, 1);
}

float AMordathBase::GetMovementSpeed() const
{
	return CurrentStageData->GetRunSpeed();
}

void AMordathBase::PlayActionMontage()
{
	PlayAnimMontage(CurrentActionMontage);
}

void AMordathBase::PlayActionMontage(UMordathActionData* ActionData)
{
	PlayAnimMontage(ActionData->ActionMontage);
}

void AMordathBase::StopActionMontage()
{
	if (!GameState->IsPlayerDead())
		StopAnimMontage();
}

void AMordathBase::OnPlayerDeath()
{
	BossAIController->StopMovement();

	FSM->RemoveAllStatesExceptActive();
}

float AMordathBase::GetDistanceToPlayer() const
{
	return (GameState->PlayerData.Location - CurrentLocation).Size();
}

FVector AMordathBase::GetDirectionToPlayer() const
{
	return (GameState->PlayerData.Location - CurrentLocation).GetSafeNormal();
}

float AMordathBase::GetActionDamage() const
{
	return ActionDamage;
}

float AMordathBase::GetAttackRadius() const
{
	return CurrentStageData->GetAttackRadius();
}

float AMordathBase::GetRecentDamage() const
{
	return CurrentStageData->GetRecentDamage();
}

bool AMordathBase::IsAttacking() const
{
	return IsShortAttacking() || IsLongAttacking() || IsSpecialAttacking();
}

bool AMordathBase::IsShortAttacking() const
{
	return false;
}

bool AMordathBase::IsLongAttacking() const
{
	return false;
}

bool AMordathBase::IsSpecialAttacking() const
{
	return false;
}
