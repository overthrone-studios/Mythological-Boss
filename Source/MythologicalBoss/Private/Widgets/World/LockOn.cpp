// Copyright Overthrone Studios 2019

#include "LockOn.h"
#include "Public/OverthroneGameInstance.h"
#include "Engine/World.h"

ALockOn::ALockOn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALockOn::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->LockOn = this;
}

void ALockOn::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

