// Copyright Overthrone Studios 2019

#include "BoundingBox.h"

#include "OverthroneGameState.h"
#include "OverthroneFunctionLibrary.h"

#include "Components/BoxComponent.h"

#include "DrawDebugHelpers.h"
#include "Log.h"

ABoundingBox::ABoundingBox()
{
#if !UE_BUILD_SHIPPING
	PrimaryActorTick.bCanEverTick = true;
#else
	PrimaryActorTick.bCanEverTick = false;
#endif

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
	
	ClearNodes();
	CreateNodes();

	const auto GameState = UOverthroneFunctionLibrary::GetGameState(this);

	GameState->PlayArea = this;
}

#if !UE_BUILD_SHIPPING
void ABoundingBox::Tick(float DeltaSeconds)
{
	for (FVector NodeLocation : Nodes)
	{
		DrawDebugSphere(GetWorld(), NodeLocation, 50.0f, 12, FColor::Green, false, -1.0f, 0, 1.0f);
	}

	DrawDebugSphere(GetWorld(), MinBoxExtent, 50.0f, 12, FColor::Red, false, -1.0f, 0, 1.0f);
	DrawDebugSphere(GetWorld(), MaxBoxExtent, 50.0f, 12, FColor::Red, false, -1.0f, 0, 1.0f);
}
#endif

#if WITH_EDITOR
void ABoundingBox::OnConstruction(const FTransform& Transform)
{
	ClearNodes();
	CreateNodes();
}

void ABoundingBox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "Rows" || PropertyChangedEvent.GetPropertyName() == "Columns")
	{
		ClearNodes();
		CreateNodes();
	}
	else if (PropertyChangedEvent.GetPropertyName() == "NodeSpacing")
	{
		ClearNodes();
		CreateNodes();
	}
	else if (PropertyChangedEvent.GetPropertyName() == "NodeZHeight")
	{
		ClearNodes();
		CreateNodes();
	}
}

bool ABoundingBox::ShouldTickIfViewportsOnly() const
{
	return true;
}
#endif

void ABoundingBox::CreateNodes()
{
	float NewSpacingX = 0.0f;
	float NewSpacingY = 0.0f;

	for (int32 y = 0; y < Columns; ++y)
	{
		for (int32 x = 0; x < Rows; ++x)
		{
			Nodes.Add(FVector(MinBoxExtent - FVector(NewSpacingX, NewSpacingY, 0.0f)));

			NewSpacingX += BoxComponent->GetUnscaledBoxExtent().X / NodeSpacing;
		}

		NewSpacingY += BoxComponent->GetUnscaledBoxExtent().Y / NodeSpacing;
		NewSpacingX = 0.0f;
	}
}

void ABoundingBox::ClearNodes()
{
	Nodes.Empty();

	MinBoxExtent = FVector(GetActorLocation().X + BoxComponent->GetUnscaledBoxExtent().X, GetActorLocation().Y + BoxComponent->GetUnscaledBoxExtent().Y, NodeZHeight);
	MaxBoxExtent = FVector(GetActorLocation().X - BoxComponent->GetUnscaledBoxExtent().X, GetActorLocation().Y - BoxComponent->GetUnscaledBoxExtent().Y, NodeZHeight);
}
