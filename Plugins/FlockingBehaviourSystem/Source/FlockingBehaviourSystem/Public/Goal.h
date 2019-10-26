// Copyright Ali El Saleh, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/MovementComponent.h"
#include "Goal.generated.h"

USTRUCT()
struct FDebugVariables_Goal
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditInstanceOnly, Category = "Debug", meta = (ToolTip = "Display the acceptance sphere?"))
		bool bShowAcceptanceRadius = false;

	UPROPERTY(EditInstanceOnly, Category = "Debug", meta = (ToolTip = "Display the path between points?"))
		bool bShowPath = false;

	UPROPERTY(EditInstanceOnly, Category = "Debug", meta = (ToolTip = "Display the max range the goal can pick from?"))
		bool bShowRangeExtent = false;

	UPROPERTY(EditInstanceOnly, Category = "Debug", meta = (ToolTip = "Display all points connected to this goal?"))
		bool bShowAllPoints = false;
};

USTRUCT()
struct FGoalSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditInstanceOnly, Category = "Settings", meta = (ClampMin=5.0f, ClampMax=10000.0f, ToolTip = "The radius in which the boid will accept that it has reached the goal location."))
		float AcceptanceRadius = 100.0f;

	UPROPERTY(EditInstanceOnly, Category = "Settings", meta = (ToolTip = "When boid has reached the goal, should we randomly generate a new location?"))
		bool bRandomlyGenerateGoalLocations = true;

	UPROPERTY(EditInstanceOnly, Category = "Settings", meta = (EditCondition = "bRandomlyGenerateGoalLocations", ToolTip = "Goal locations will generate within the bounds of this box"))
		FVector Range = FVector(1000.0f, 1000.0f, 500.0f);

	UPROPERTY(EditInstanceOnly, Category = "Settings", meta = (EditCondition = "bRandomlyGenerateGoalLocations", ToolTip = "Should we start at a custom location in world space?"))
		bool bStartAtCustomLocation = false;

	UPROPERTY(EditInstanceOnly, Category = "Settings", meta = (EditCondition = "bStartAtCustomLocation", ToolTip = "The custom location the goal should start at in world space."))
		FVector StartLocation = FVector(0.0f);

	UPROPERTY(EditInstanceOnly, Category = "Settings", meta = (EditCondition = "!bRandomlyGenerateGoalLocations", ToolTip = "Should we spawn a new Point actor when creating a new element of the array?"))
		bool bSpawnNewPointWhenAddingToArray = false;

	UPROPERTY(EditInstanceOnly, Category = "Settings", meta = (EditCondition = "!bRandomlyGenerateGoalLocations", ToolTip = "A list of pre-determined goal locations for the boid to move to. (Will return to first index when completed)"))
		TArray<class APoint*> GoalLocations;
};

UCLASS(HideCategories = ("Rendering", "Input", "Actor", "LOD", "Cooking"))
class FLOCKINGBEHAVIOURSYSTEM_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoal();

	void ToggleVisibility(bool bIsVisible);

	void FindNewLocation(EPlaneConstraintAxisSetting PlaneConstraintAxis);
	void SetNewGoalLocation(FVector NewLocation);
	void NextLocation();

	FBox GetBox() const;
	FVector GetRange() const;
	FVector GetGoalLocation() const;
	FVector GetOriginalLocation() const;
	uint32 GetTotalLocations() const;
	float GetAcceptanceRadius() const;
	bool IsRandomlyGenerating() const;

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	#if WITH_EDITOR
	void OnConstruction(const FTransform& Transform) override;
	bool ShouldTickIfViewportsOnly() const override;
	void PostEditMove(bool bFinished) override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UBillboardComponent* BillboardComponent;

	UPROPERTY(VisibleAnywhere, Category = "Properties", meta = (ToolTip = "The location that the boid will seek towards"))
		FVector GoalLocation;

	UPROPERTY(EditInstanceOnly, Category = "Properties", meta = (ToolTip = "Adjust goal settings"))
		FGoalSettings Settings;

	UPROPERTY(EditInstanceOnly, Category = "Properties", meta = (ToolTip = "Visualize debug information"))
		FDebugVariables_Goal Debug;

	FBox Box;

private:
	void SpawnPointActor();
	void DestroyAllPointActorsOwnedByThis();
	void DestroyNullPointRefInArray();
	void TogglePointVisibility();

	FVector OriginalLocation{};
	uint32 GoalLocationIndex{};

	UTexture2D* BillboardSprite;
};
