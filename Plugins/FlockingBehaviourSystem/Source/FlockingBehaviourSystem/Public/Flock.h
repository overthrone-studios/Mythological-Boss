// Copyright Ali El Saleh, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/MovementComponent.h"
#include "FlockWorker.h"
#include "Flock.generated.h"

USTRUCT()
struct FBoidSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", DisplayName = "Mesh", meta = (ToolTip = "The static mesh to represent the boid"))
		UStaticMesh* BoidMesh{};

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", DisplayName = "Materials", meta = (ToolTip = "The material/s that matches the boid mesh"))
		TArray<UMaterialInterface*> BoidMaterials{};

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ClampMin = 0.01f, ToolTip = "Scale the mesh by the multiplier"))
		float ScaleMultiplier = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ToolTip = "The default rotation of the mesh (You will need to experiment which way your mesh should face)"))
		FRotator Rotation = FRotator(90.0f, 180.0f, 0.0f);

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ToolTip = "Randomize the speed of each boid?"))
		bool bRandomSpeed = false;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "!bRandomSpeed", ClampMin = 1.0f, ToolTip = "The maximum movement speed of each boid"))
		float Speed = 1200.0f;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "bRandomSpeed", ClampMin = 1.0f, ToolTip = "The minimum range, when RandomSpeed is enabled"))
		float MinSpeed = 500.0f;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "bRandomSpeed", ClampMin = 1.0f, ToolTip = "The maximum range, when RandomSpeed is enabled"))
		float MaxSpeed = 2500.0f;

	// Randomize the perception radius of each boid?
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		bool bRandomPerceptionRadius = false;

	// How far can each boid see?
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "!bRandomPerceptionRadius", ClampMin = 1.0f))
		float PerceptionRadius = 300.0f;

	// The minimum range of how far each boid see?
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "bRandomPerceptionRadius", ClampMin = 1.0f))
		float MinPerceptionRadius = 100.0f;

	// The maximum range of how far each boid see?
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "bRandomPerceptionRadius", ClampMin = 1.0f))
		float MaxPerceptionRadius = 300.0f;

	// The distance from the boid's location
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ClampMin = 1.0f))
		float AvoidanceDistance = 2000.0f;

	// How fast can each boid turn?
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ClampMin = 0.0f, ClampMax = 40.0f))
		float TurnSpeed = 10.0f;

	// The distance from the spawn location in Unreal units (cm)
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ClampMin = 100.0f))
		float WanderDistance = 2000.0f;

	// Constraint movement to a plane?
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		bool bConstrainToPlane = false;

	// Restrict movement to a specific axis
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "bConstrainToPlane"))
		EPlaneConstraintAxisSetting PlaneConstraintAxis;
};

USTRUCT()
struct FSpawnSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ToolTip = "The amount of boids to spawn for this flock", ClampMin = 2, ClampMax = 400))
		uint32 NumberOfBoids = 400;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (ToolTip = "The spawning area that each boid will be randomly placed in", ClampMin = 100.0f, ClampMax = 2000.0f))
		float SpawnRadius = 1000.0f;
};

USTRUCT()
struct FFlockSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", DisplayName = "Randomize Forces?", meta = (ToolTip = "Randomise all forces at begin play?"))
		bool bRandomForces = false;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "!bRandomForces", ClampMin = 0.0f, ClampMax = 10.0f, ToolTip = "How strong is the alignment force? (How easy is it to align with other boids)"))
		float AlignmentForce = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "!bRandomForces", ClampMin = 0.0f, ClampMax = 10.0f, ToolTip = "How strong is the cohesion force? (How easy is it to cohere with other boids?)"))
		float CohesionForce = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "!bRandomForces", ClampMin = 0.0f, ClampMax = 10.0f, ToolTip = "How strong is the separation force? (How easy is it to separate from other boids?)"))
		float SeparationForce = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", DisplayName = "Allow Obstacle Avoidance?", meta = (ToolTip = "Allow each boid in this flock to avoid obstacles?"))
		bool bAllowObstacleAvoidance = false;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "bAllowObstacleAvoidance", ClampMin = 0.0f, ClampMax = 3.0f, ToolTip = "How strong is the obstacle avoidance force? (How easy is it to avoid obstacles?)"))
		float AvoidanceForce = 1.0f;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", DisplayName = "Enable Goal Seeking?", meta = (ToolTip = "Enable/Disable the goal seeking behavior?"))
		bool bEnableGoalSeeking = false;

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", DisplayName = "Goal Actor", meta = (EditCondition = "bEnableGoalSeeking", ToolTip = "The goal actor in the scene that this flock will seek toward."))
		class AGoal* Goal{};

	UPROPERTY(EditInstanceOnly, Category = "Flock Properties", meta = (EditCondition = "bEnableGoalSeeking", ClampMin = 0.0f, ClampMax = 10.0f, ToolTip = "How strong is the goal seeking force?"))
		float GoalSeekingForce = 1.0f;
};

USTRUCT()
struct FFlockDebugSettings
{
	GENERATED_USTRUCT_BODY()

	// Visualize the avoidance force?
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		uint8 bShowAvoidanceVector : 1;

	// Visualize the avoidance distance? (The nose of the boid)
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		uint8 bShowAvoidanceDistance : 1;
};

/*
 A flock is an actor that contains 2 or more boids and will flock with each other when they can see one another.
 */
UCLASS(HideCategories=("Rendering", "Input", "Actor", "LOD", "Cooking"))
class FLOCKINGBEHAVIOURSYSTEM_API AFlock : public AActor
{
	GENERATED_BODY()

public:
	AFlock();

	void AddBoid();

protected:
	#pragma region Virtual Functions
	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
	#pragma endregion

	#pragma region Functions
	void ApplyBoidSettings(ABoid* Boid) const;
	void ApplyFlockSettings(ABoid* Boid);
	void ApplyBoidDebugSettings(ABoid* Boid);

	void CreateNewThread();
	#pragma endregion

	#pragma region Components
		UPROPERTY(EditDefaultsOnly, Category = "Components")
			class USceneComponent* SceneComponent;
		UPROPERTY(EditDefaultsOnly, Category = "Components")
			class UBillboardComponent* BillboardComponent;
	#pragma endregion

	#pragma region Properties
	// Customize boid attributes
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		FBoidSettings BoidSettings;

	// Specify the spawning rules
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		FSpawnSettings SpawnSettings;

	// Customize the flocking behavior rules
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		FFlockSettings FlockSettings;

	// Debug settings for each boid in this flock
	UPROPERTY(EditInstanceOnly, Category = "Flock Properties")
		FFlockDebugSettings Debug;
	#pragma endregion

private:
	// The array of boids
	TArray<class ABoid*> Boids;
	
	// Thread
	FlockWorker* Worker{};

	// The sprite that can be seen in the editor
	UTexture2D* BillboardSprite;
};
