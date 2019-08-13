// Copyright Overthrone Studios 2019

#include "OverthroneCharacter.h"
#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneHUD.h"
#include "Public/FSM.h"
#include "HUD/MasterHUD.h"
#include "HUD/FSMVisualizerHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AOverthroneCharacter::AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AOverthroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bCanBeDamaged = true;

	Health = StartingHealth;

	// Cache the world object
	World = GetWorld();

	// Cache the player controller
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Cache the movement component
	MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = MovementSettings.WalkSpeed;

	// Cache our anim instance todo
	//AnimInstance = Cast<UYlvaAnimInstance>(GetMesh()->GetAnimInstance());

	// Cache the player HUD
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	// Cache our game instance reference
	GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));

	// Cache the FSM Visualizer HUD
	FSMVisualizer = Cast<UFSMVisualizerHUD>(OverthroneHUD->GetMasterHUD()->GetHUD(UFSMVisualizerHUD::StaticClass()));

	// Begin the state machine
	FSM->Start();
}

void AOverthroneCharacter::UpdateCharacterInfo()
{
	check(0 && "You must implement UpdateCharacterInfo()");
}

void AOverthroneCharacter::SetHealth(const float NewHealthAmount)
{
	Health = FMath::Clamp(NewHealthAmount, 0.0f, StartingHealth);
	//GameInstance->PlayerInfo.Health = Health;
	UpdateCharacterInfo();
}

void AOverthroneCharacter::IncreaseHealth(const float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, StartingHealth);
	//GameInstance->PlayerInfo.Health = Health;
	UpdateCharacterInfo();
}

void AOverthroneCharacter::DecreaseHealth(const float Amount)
{
	Health = FMath::Clamp(Health - Amount, 0.0f, StartingHealth);
	//GameInstance->PlayerInfo.Health = Health;
	UpdateCharacterInfo();
}

void AOverthroneCharacter::ResetHealth()
{
	Health = StartingHealth;
	//GameInstance->PlayerInfo.Health = Health;
	UpdateCharacterInfo();
}

void AOverthroneCharacter::Die()
{
	SetHealth(0.0f);

	bCanBeDamaged = false;

	//AnimInstance->LeaveAllStates();
}

void AOverthroneCharacter::PauseAnims()
{
	GetMesh()->bPauseAnims = true;
}

void AOverthroneCharacter::UnPauseAnims()
{
	GetMesh()->bPauseAnims = false;
}

bool AOverthroneCharacter::IsInvincible()
{
	return !bCanBeDamaged;
}

