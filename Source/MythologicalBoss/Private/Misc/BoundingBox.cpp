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

FBox ABoundingBox::GetBoundingBox() const
{
	return FBox(FVector(BoxComponent->GetScaledBoxExtent().GetMin()), FVector(BoxComponent->GetUnscaledBoxExtent().GetMax()));
}

void ABoundingBox::BeginPlay()
{
	Super::BeginPlay();
	
	const auto GameState = UOverthroneFunctionLibrary::GetGameState(this);

	GameState->PlayArea = this;
}
