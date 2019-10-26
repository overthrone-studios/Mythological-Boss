// Copyright Ali El Saleh, 2019

#include "Goal.h"
#include "Point.h"
#include "Components/BillboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AGoal::AGoal()
{
	PrimaryActorTick.bCanEverTick = true;

	// Scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Default Scene Root"));
	RootComponent = SceneComponent;

	// Billboard component
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard"));
	BillboardComponent->SetupAttachment(SceneComponent);
	BillboardComponent->SetRelativeLocation(FVector(0.0f));
	BillboardComponent->bIsScreenSizeScaled = true;

	BillboardSprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Engine/EditorResources/Waypoint.Waypoint'")));
	if (BillboardSprite)
		BillboardComponent->SetSprite(BillboardSprite);
}

#if WITH_EDITOR
void AGoal::ToggleVisibility(const bool bIsVisible)
{
	if (!Settings.bRandomlyGenerateGoalLocations)
	{
		Debug.bShowPath = bIsVisible;
		Debug.bShowAllPoints = bIsVisible;

		TogglePointVisibility();
	}
	else
	{
		Debug.bShowPath = false;
		Debug.bShowAllPoints = false;
		TogglePointVisibility();

		Debug.bShowRangeExtent = bIsVisible;
	}

	if (Debug.bShowAcceptanceRadius)
		Debug.bShowAcceptanceRadius = false;
}
#endif

void AGoal::FindNewLocation(const EPlaneConstraintAxisSetting PlaneConstraintAxis)
{
	float X = 0.0f, Y = 0.0f, Z = 0.0f;

	switch (PlaneConstraintAxis)
	{
		case EPlaneConstraintAxisSetting::X:
		Y = FMath::FRandRange(OriginalLocation.Y - GetRange().Y, OriginalLocation.Y + GetRange().Y);
		Z = FMath::FRandRange(OriginalLocation.Z - GetRange().Z, OriginalLocation.Z + GetRange().Z);
		break;
	
		case EPlaneConstraintAxisSetting::Y:
		X = FMath::FRandRange(OriginalLocation.X - GetRange().X, OriginalLocation.X + GetRange().X);
		Z = FMath::FRandRange(OriginalLocation.Z - GetRange().Z, OriginalLocation.Z + GetRange().Z);
		break;
	
		case EPlaneConstraintAxisSetting::Z:
		X = FMath::FRandRange(OriginalLocation.X - GetRange().X, OriginalLocation.X + GetRange().X);
		Y = FMath::FRandRange(OriginalLocation.Y - GetRange().Y, OriginalLocation.Y + GetRange().Y);
		break;
	
		default:
		X = FMath::FRandRange(OriginalLocation.X - GetRange().X, OriginalLocation.X + GetRange().X);
		Y = FMath::FRandRange(OriginalLocation.Y - GetRange().Y, OriginalLocation.Y + GetRange().Y);
		Z = FMath::FRandRange(OriginalLocation.Z - GetRange().Z, OriginalLocation.Z + GetRange().Z);
		break;
	}

	SetNewGoalLocation(FVector(X, Y, Z));
}

void AGoal::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GoalLocation;

	if (Settings.GoalLocations.Num() > 0)
		Settings.GoalLocations[0]->SetActorTransform(GetActorTransform());

	DestroyNullPointRefInArray();
}

void AGoal::Tick(float DeltaSeconds)
{
	if (Debug.bShowAcceptanceRadius)
		DrawDebugSphere(GetWorld(), GoalLocation, Settings.AcceptanceRadius, 20, FColor::Red);

	if (Debug.bShowPath)
	{
		for (int32 i = 0; i < Settings.GoalLocations.Num(); i++)
		{
			if (i != Settings.GoalLocations.Num() - 1)
				if (Settings.GoalLocations[i+1])
					DrawDebugLine(GetWorld(), Settings.GoalLocations[i]->GetActorLocation(), Settings.GoalLocations[i+1]->GetActorLocation(), FColor::Magenta, false, -1, 0, 3.0f);
		}
	}

	Box.Max = Settings.Range;
	Box.Min = -Settings.Range;

	if (Debug.bShowRangeExtent)
		DrawDebugBox(GetWorld(), OriginalLocation, Box.GetExtent(), FColor::Magenta, false, -1, 0, 10.0f);
}
#if WITH_EDITOR
bool AGoal::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AGoal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Settings.bStartAtCustomLocation)
		GoalLocation = Settings.StartLocation;
	else
		GoalLocation = Transform.GetLocation();

	OriginalLocation = GoalLocation;
}

void AGoal::PostEditMove(const bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (Settings.GoalLocations.Num() > 0)
		Settings.GoalLocations[0]->SetActorLocation(GetActorLocation());

	OriginalLocation = GoalLocation;
}
#endif

void AGoal::TogglePointVisibility()
{
	for (auto Point : Settings.GoalLocations)
	{
		Point->SetVisibility(Debug.bShowAllPoints);
	}
}

#if WITH_EDITOR
void AGoal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "GoalLocations")
	{
		switch (PropertyChangedEvent.ChangeType)
		{
			case EPropertyChangeType::ArrayAdd:
			{
				if (Settings.bSpawnNewPointWhenAddingToArray)
				{
					// To remove the extra element from the editor
					Settings.GoalLocations.Pop();

					SpawnPointActor();
				}
			}
			break;
		
			case EPropertyChangeType::ArrayClear:
			{
				DestroyAllPointActorsOwnedByThis();

				SpawnPointActor();
			}
			break;

			case EPropertyChangeType::ValueSet:
			{
				for (auto Point : Settings.GoalLocations)
				{
					if (Point)
						Point->SetOwningGoal(this);
				}
			}
			break;

			default:
			break;
		}
	}
	else if (PropertyChangedEvent.GetPropertyName() == "bRandomlyGenerateGoalLocations")
	{
		Debug.bShowPath = !Settings.bRandomlyGenerateGoalLocations;
		Debug.bShowRangeExtent = Settings.bRandomlyGenerateGoalLocations;
		Debug.bShowAllPoints = !Settings.bRandomlyGenerateGoalLocations;

		TogglePointVisibility();
	}
	else if (PropertyChangedEvent.GetPropertyName() == "bShowPath")
	{
		if (Settings.bRandomlyGenerateGoalLocations)
			Debug.bShowPath = false;
	}
	else if (PropertyChangedEvent.GetPropertyName() == "bShowRangeExtent")
	{
		if (!Settings.bRandomlyGenerateGoalLocations)
			Debug.bShowRangeExtent = false;
	}
	else if (PropertyChangedEvent.GetPropertyName() == "bShowAllPoints")
	{
		TogglePointVisibility();
	}
}
#endif

FVector AGoal::GetGoalLocation() const
{
	return GoalLocation;
}

FVector AGoal::GetOriginalLocation() const
{
	return OriginalLocation;
}

void AGoal::SetNewGoalLocation(const FVector NewLocation)
{
	GoalLocation = NewLocation;
}

void AGoal::NextLocation()
{
	if (Settings.GoalLocations.Num() > 0)
	{
		if (GoalLocation.Equals(Settings.GoalLocations[Settings.GoalLocations.Num() - 1]->GetActorLocation(), 0.01f))
		{
			GoalLocation = Settings.GoalLocations[0]->GetActorLocation();
			GoalLocationIndex = 0;
		}
		else
		{
			GoalLocation = Settings.GoalLocations[GoalLocationIndex]->GetActorLocation();
			GoalLocationIndex++;
		
			if (GoalLocationIndex >= uint32(Settings.GoalLocations.Num()))
				GoalLocationIndex = 0;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: There are no goal locations to seek towards. Will now randomly generate locations..."), *GetName());

		Settings.bRandomlyGenerateGoalLocations = true;
	}
}

FBox AGoal::GetBox() const
{
	return Box;
}

FVector AGoal::GetRange() const
{
	return Settings.Range;
}

uint32 AGoal::GetTotalLocations() const
{
	return uint32(Settings.GoalLocations.Num());
}

float AGoal::GetAcceptanceRadius() const
{
	return Settings.AcceptanceRadius;
}

bool AGoal::IsRandomlyGenerating() const
{
	return Settings.bRandomlyGenerateGoalLocations;
}

void AGoal::SpawnPointActor()
{
	const auto NewPoint = GetWorld()->SpawnActor<APoint>(APoint::StaticClass(), GetActorTransform());
	NewPoint->ID = Settings.GoalLocations.Num() + 1;

#if WITH_EDITOR
	NewPoint->SetActorLabel(FString("Point_") + FString::FromInt(NewPoint->ID));
#endif

	NewPoint->SetOwningGoal(this);

	Settings.GoalLocations.Add(NewPoint);
}

void AGoal::DestroyAllPointActorsOwnedByThis()
{
	TArray<AActor*> FoundPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APoint::StaticClass(), FoundPoints);

	for (auto Point : FoundPoints)
	{
		if (Cast<APoint>(Point)->GetGoalOwner() == this)
			Point->K2_DestroyActor();
	}
}

void AGoal::DestroyNullPointRefInArray()
{
	for (int32 i = 0; i < Settings.GoalLocations.Num(); i++)
	{
		if (!Settings.GoalLocations[i])
		{
			Settings.GoalLocations.RemoveAt(i);
			i--;
		}
	}
	
	Settings.GoalLocations.Shrink();
}
