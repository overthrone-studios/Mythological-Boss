// Copyright Ali El Saleh, 2019

#include "Flock.h"
#include "Goal.h"
#include "Boid.h"
#include "FlockWorker.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"

AFlock::AFlock()
{
	PrimaryActorTick.bCanEverTick = false;

	// Scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Default Scene Root"));
	RootComponent = SceneComponent;

	// Billboard component
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard"));
	BillboardComponent->SetupAttachment(SceneComponent);
	BillboardComponent->SetRelativeLocation(FVector(0.0f));
	BillboardComponent->SetRelativeScale3D(FVector(2.0f));
	BillboardComponent->bIsScreenSizeScaled = true;

	BillboardSprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/FlockingBehaviourSystem/FlockIcon.FlockIcon'")));
	if (BillboardSprite)
		BillboardComponent->SetSprite(BillboardSprite);

	BoidSettings.BoidMaterials.AddDefaulted();
}

void AFlock::BeginPlay()
{
	Super::BeginPlay();

	// Spawn 'X' number of boids into world
	for (uint32 i = 0; i < SpawnSettings.NumberOfBoids; i++)
		AddBoid();

	if (!BoidSettings.BoidMesh)
		UE_LOG(LogTemp, Warning, TEXT("No static mesh selected. Will default back to cone mesh!"))

	//CreateNewThread();
}

void AFlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Destroy the worker thread
	//Worker->Shutdown();
}

#if WITH_EDITOR
void AFlock::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "bEnableGoalSeeking")
	{
		if (FlockSettings.Goal)
			FlockSettings.Goal->ToggleVisibility(FlockSettings.bEnableGoalSeeking);
	}
	else if (PropertyChangedEvent.GetPropertyName() == "SpawnRadius")
	{
		if (SpawnSettings.SpawnRadius > BoidSettings.WanderDistance)
			BoidSettings.WanderDistance = SpawnSettings.SpawnRadius;
	}
}
#endif

void AFlock::ApplyBoidSettings(ABoid* Boid) const
{
	Boid->AssignMesh(BoidSettings.BoidMesh, BoidSettings.BoidMaterials);
	Boid->SetMeshScale(BoidSettings.ScaleMultiplier);
	Boid->SetMeshRotation(BoidSettings.Rotation);
	Boid->SetTurnSpeed(BoidSettings.TurnSpeed);
	Boid->SetWanderDistance(BoidSettings.WanderDistance);
	Boid->EnablePlaneConstraint(BoidSettings.bConstrainToPlane);
	Boid->SetPlaneConstraintAxis(BoidSettings.PlaneConstraintAxis);
	Boid->SetAvoidanceDistance(BoidSettings.AvoidanceDistance);

	if (BoidSettings.bRandomSpeed)
		Boid->SetSpeed(FMath::FRandRange(BoidSettings.MinSpeed, BoidSettings.MaxSpeed));
	else
		Boid->SetSpeed(BoidSettings.Speed);

	if (BoidSettings.bRandomPerceptionRadius)
		Boid->SetPerceptionRadius(FMath::FRandRange(BoidSettings.MinPerceptionRadius, BoidSettings.MaxPerceptionRadius));
	else
		Boid->SetPerceptionRadius(BoidSettings.PerceptionRadius);
}

void AFlock::ApplyFlockSettings(ABoid* Boid)
{
	Boid->SetFlock(this);

	if (FlockSettings.bRandomForces)
	{
		FlockSettings.AlignmentForce = FMath::FRandRange(1.0f, 10.0f);
		FlockSettings.CohesionForce = FMath::FRandRange(1.0f, 10.0f);
		FlockSettings.SeparationForce = FMath::FRandRange(1.0f, 10.0f);
		FlockSettings.AvoidanceForce = FMath::FRandRange(1.0f, 3.0f);
	}

	Boid->SetAlignmentForce(FlockSettings.AlignmentForce);
	Boid->SetCohesionForce(FlockSettings.CohesionForce);
	Boid->SetSeparationForce(FlockSettings.SeparationForce);
	Boid->SetAvoidanceForce(FlockSettings.AvoidanceForce);	
	Boid->SetAllowAvoidanceBehavior(FlockSettings.bAllowObstacleAvoidance);
	Boid->EnableGoalSeeking(FlockSettings.bEnableGoalSeeking);

	if (FlockSettings.bEnableGoalSeeking)
		Boid->SetGoalSeekingForce(FlockSettings.GoalSeekingForce);

	if (FlockSettings.Goal)
		Boid->SetGoal(FlockSettings.Goal);
}

void AFlock::ApplyBoidDebugSettings(ABoid* Boid)
{
	Boid->SetShowAvoidanceForce(Debug.bShowAvoidanceVector);
	Boid->SetShowAvoidanceDistance(Debug.bShowAvoidanceDistance);
	//Boid->SetShowPerceptionRadius(Debug.bShowAvoidanceDistance); Todo
}

void AFlock::AddBoid()
{
	// Lambda function, to generate a random point on sphere (this is used for spawning boids on the generated point)
	const auto GetRandomPointOnSphere = [](const FVector Center, const float Radius)
	{
		const float Angle = FMath::FRandRange(0.0f, PI);
		const float Phi = FMath::FRandRange(0.0f, 2 * PI);

		const float X = Center.X + Radius * FMath::Sin(Angle) * FMath::Cos(Phi);
		const float Y = Center.Y + Radius * FMath::Sin(Angle) * FMath::Sin(Phi);
		const float Z = Center.Z + Radius * FMath::Cos(Angle);

		return FVector(X, Y, Z);
	};

	// Spawn a new boid into the world
	const auto Boid = GetWorld()->SpawnActor<ABoid>(ABoid::StaticClass(), GetRandomPointOnSphere(GetActorLocation(), SpawnSettings.SpawnRadius), BoidSettings.Rotation);
	Boid->SpawnDefaultController();
	
	// Set its properties
	ApplyBoidSettings(Boid);
	ApplyFlockSettings(Boid);
	ApplyBoidDebugSettings(Boid);

	// Add to the array of boids
	Boids.AddUnique(Boid);
}

void AFlock::CreateNewThread()
{
	Worker = new FlockWorker(&Boids);
}
