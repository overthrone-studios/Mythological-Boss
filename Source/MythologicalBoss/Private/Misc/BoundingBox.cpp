// Copyright Overthrone Studios 2019

#include "BoundingBox.h"
#include "OverthroneGameInstance.h"
#include "Components/BoxComponent.h"


ABoundingBox::ABoundingBox()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoundingBox"));
	RootComponent = BoxComponent;
}

void ABoundingBox::BeginPlay()
{
	Super::BeginPlay();
	
	Box.Min = -BoxComponent->GetUnscaledBoxExtent();
	Box.Max = BoxComponent->GetUnscaledBoxExtent();

	const auto GameInstance = Cast<UOverthroneGameInstance>(GetGameInstance());

	GameInstance->PlayArea = Box;
}
