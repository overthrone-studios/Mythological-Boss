// Copyright Overthrone Studios 2019

#include "Ylva.h"


// Sets default values
AYlva::AYlva()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AYlva::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AYlva::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AYlva::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

