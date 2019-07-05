// Copyright Ali El Saleh 2019

#include "MenuPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMenuPawn::AMenuPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	// Camera component setup
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("Camera Component"));
	RootComponent = CameraComponent;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	EnableUIMode();
}

// Called to bind functionality to input
void AMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMenuPawn::EnableUIMode() const
{
	const FInputModeUIOnly InputModeUIOnly;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputModeUIOnly);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}

