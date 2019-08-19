// Copyright Overthrone Studios 2019

#include "OverthroneCharacter.h"
#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneAnimInstance.h"
#include "Public/OverthroneHUD.h"
#include "HUD/FSMVisualizerHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AOverthroneCharacter::AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->bReturnMaterialOnMove = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	bCanBeDamaged = true;
}

void AOverthroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Store all our child components
	Components = GetComponents();

	// Become vulnerable
	bCanBeDamaged = true;

	Health = StartingHealth;
	PreviousHealth = Health;

	// Initialize variables
	World = GetWorld();
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	MovementComponent = GetCharacterMovement();
	AnimInstance = Cast<UOverthroneAnimInstance>(GetMesh()->GetAnimInstance());
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void AOverthroneCharacter::UpdateCharacterInfo()
{
	check(0 && "You must implement UpdateCharacterInfo()");
}

void AOverthroneCharacter::ApplyHitStop()
{

}

void AOverthroneCharacter::ChangeHitboxSize(float NewRadius)
{
	check(0 && "You must implement ChangeHitboxSize()");
}

void AOverthroneCharacter::OnLowHealth()
{
}

void AOverthroneCharacter::StartLosingHealth(const float Amount)
{
	PreviousHealth = Health;
	Health -= Amount;
}

void AOverthroneCharacter::LoseHealth()
{
	UpdateCharacterInfo();
	
	if (Health <= StartingHealth * LowHealthThreshold && !bWasLowHealthEventTriggered)
	{
		BroadcastLowHealth();
	}
}

void AOverthroneCharacter::FinishLosingHealth()
{
	Health = NewHealth;
	PreviousHealth = Health;

	UpdateCharacterInfo();
}

void AOverthroneCharacter::SetHealth(const float NewHealthAmount)
{
	PreviousHealth = Health;

	Health = FMath::Clamp(NewHealthAmount, 0.0f, StartingHealth);

	UpdateCharacterInfo();

	if (Health <= StartingHealth * LowHealthThreshold && !bWasLowHealthEventTriggered)
	{
		BroadcastLowHealth();
	}
}

void AOverthroneCharacter::IncreaseHealth(const float Amount)
{
	PreviousHealth = Health;

	Health = FMath::Clamp(Health + Amount, 0.0f, StartingHealth);

	UpdateCharacterInfo();
}

void AOverthroneCharacter::DecreaseHealth(const float Amount)
{
	PreviousHealth = Health;

	Health = FMath::Clamp(Health - Amount, 0.0f, StartingHealth);

	UpdateCharacterInfo();

	if (Health <= StartingHealth * LowHealthThreshold && !bWasLowHealthEventTriggered)
	{
		BroadcastLowHealth();
	}
}

void AOverthroneCharacter::ResetHealth()
{
	Health = StartingHealth;
	PreviousHealth = Health;

	UpdateCharacterInfo();
}

void AOverthroneCharacter::Die()
{
	bCanBeDamaged = false;

	AnimInstance->LeaveAllStates();
}

void AOverthroneCharacter::BroadcastLowHealth()
{
	check(0 && "You must implement BroadcastLowHealth()");
}

void AOverthroneCharacter::PauseAnims() const
{
	GetMesh()->bPauseAnims = true;
}

void AOverthroneCharacter::UnPauseAnims() const
{
	GetMesh()->bPauseAnims = false;
}

bool AOverthroneCharacter::IsInvincible() const
{
	return !bCanBeDamaged;
}
