// Copyright Overthrone Studios 2019

#include "BoundingBox.h"

#include "OverthroneGameState.h"
#include "OverthroneFunctionLibrary.h"

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

	const auto GameState = UOverthroneFunctionLibrary::GetGameState(this);

	GameState->PlayArea = Box;
}
