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

}

void AMenuPawn::PossessedBy(AController* NewController)
{
	PlayerController = Cast<APlayerController>(NewController);

	EnableUIMode();
}

void AMenuPawn::EnableUIMode() const
{
	const FInputModeUIOnly InputModeUIOnly;
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
}

