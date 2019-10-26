// Copyright Ali El Saleh, 2019

#include "Point.h"
#include "Goal.h"
#include "Components/BillboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Texture2D.h"

APoint::APoint()
{
	PrimaryActorTick.bCanEverTick = false;

	// Scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Default Scene Root"));
	RootComponent = SceneComponent;

	// Billboard component
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard"));
	BillboardComponent->SetupAttachment(SceneComponent);
	BillboardComponent->SetRelativeLocation(FVector(0.0f));
	BillboardComponent->bIsScreenSizeScaled = true;

	BillboardSprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Engine/EngineDebugMaterials/TargetIcon.TargetIcon'")));
	if (BillboardSprite)
		BillboardComponent->SetSprite(BillboardSprite);
}

void APoint::SetVisibility(const bool bIsVisible) const
{
	BillboardComponent->SetVisibility(bIsVisible);
}

void APoint::SetOwningGoal(AGoal* Goal)
{
	OwningGoal = Goal;
}

AGoal* APoint::GetGoalOwner() const
{
	return OwningGoal;
}

void APoint::BeginPlay()
{
	Super::BeginPlay();

	if (!OwningGoal)
		UE_LOG(LogTemp, Error, TEXT("%s is not owned by any goal actor. Assign this point to a goal."), *GetName())
}

#if WITH_EDITOR
void APoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	BillboardComponent->SetVisibility(bVisible);
}
#endif
