// Copyright Overthrone Studios 2019

#include "LockOn.h"
#include "Public/OverthroneGameInstance.h"
#include "Engine/World.h"

ALockOn::ALockOn()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ALockOn::BeginPlay()
{
	Super::BeginPlay();

	class UOverthroneGameInstance* GameInstance = Cast<UOverthroneGameInstance>(GetWorld()->GetGameInstance());

	GameInstance->LockOn = this;
}
