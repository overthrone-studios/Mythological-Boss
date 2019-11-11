// Copyright Ali El Saleh, 2019

#include "Boid.h"

#include "BoidController.h"
#include "Goal.h"
#include "Flock.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "BoidMovementComponent.h"

#include "Engine/StaticMesh.h"

#include "DrawDebugHelpers.h"

ABoid::ABoid()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	RootComponent = SceneComponent;

	// Static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	// Object loading
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'")));
	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'")));

	// Static mesh component settings
	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetMaterial(0, Material);
	}
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetEnableGravity(false);
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	StaticMeshComponent->SetCastShadow(false);
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
	StaticMeshComponent->bApplyImpulseOnDamage = false;

	// Sphere component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("Sphere Component"));
	SphereComponent->SetComponentTickEnabled(false);
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(PerceptionRadius, true);
	SphereComponent->SetEnableGravity(false);
	SphereComponent->bApplyImpulseOnDamage = false;
	SphereComponent->CanCharacterStepUpOn = ECB_No;
	SphereComponent->AreaClass = nullptr;
	SphereComponent->SetCanEverAffectNavigation(false);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABoid::OnBoidDetected);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABoid::OnBoidLost);

	// Movement component
	BoidMovementComponent = CreateDefaultSubobject<UBoidMovementComponent>(TEXT("BoidMovementComponent"));
	BoidMovementComponent->UpdatedComponent = RootComponent;
	BoidMovementComponent->SetUpdateNavAgentWithOwnersCollisions(false);
	BoidMovementComponent->SetCanEverAffectNavigation(false);

	// Custom default pawn settings
	AIControllerClass = ABoidController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationYaw = true;
}

void ABoid::BeginPlay()
{
	Super::BeginPlay();

	SpawnLocation = GetActorLocation();
	Rotation = GetActorRotation();

	SphereComponent->SetSphereRadius(PerceptionRadius, true);

	World = GetWorld();

	if (!BoidMovementComponent->bConstrainToPlane)
		BoidMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Custom);
}

void ABoid::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FVector ABoid::SeekTowardsGoal()
{
	// Get the distance between this and the goal location
	const float Distance = FVector::Dist(GetActorLocation(), Behaviors.GoalSeeking.Goal->GetGoalLocation());

	// Are we inside the acceptance radius
	if (Distance < Behaviors.GoalSeeking.Goal->GetAcceptanceRadius())
	{
		if (Behaviors.GoalSeeking.Goal->IsRandomlyGenerating())
		{
			// Go to next random goal
			if (BoidMovementComponent->bConstrainToPlane)
				Behaviors.GoalSeeking.Goal->FindNewLocation(BoidMovementComponent->GetPlaneConstraintAxisSetting());
			else
				Behaviors.GoalSeeking.Goal->FindNewLocation(EPlaneConstraintAxisSetting::Custom);
		}
		else
		{
			// Go to next goal in the array
			Behaviors.GoalSeeking.Goal->NextLocation();
		}
	}

	// Get the steering vector
	FVector Steer = Seek(Behaviors.GoalSeeking.Goal->GetGoalLocation());

	Steer *= bShouldAvoid ? 0.0f : 1.0f;

	return Steer;
}

bool ABoid::HasWanderedTooFar()
{	
	// If the boid is in the wandering area
	return FVector::Dist(GetActorLocation(), SpawnLocation) > WanderDistance;
}

void ABoid::CheckForObstacles()
{
	// Raycast information
	FHitResult OutHit;
	bool bHit = false;
	const FVector LineEnd = GetActorLocation() + GetVelocity().GetSafeNormal() * AvoidanceDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	// Do the raycasting
	if (bShouldRaycast)
		bHit = GetWorld()->LineTraceSingleByObjectType(OutHit, GetActorLocation(), LineEnd, ECC_WorldStatic, CollisionQueryParams);

	#if !UE_BUILD_SHIPPING
		if (Debug.bShowAvoidanceDistance)
			DrawDebugLine(World, GetActorLocation(), LineEnd, FColor::Orange, false, -1.0f, 0, 2.0f);
	#endif

	// If we have hit an obstacle
	if (bHit)
	{
		bShouldRaycast = false;

		// The actor that was hit
		Obstacle = OutHit.GetActor();

		// Store the hit location as the avoid location
		AvoidLocation = OutHit.Location;

		if (!bShouldAvoid)
		{
			bShouldAvoid = true;
		}
	}

	// Calculate the distance between this boid and the avoid location. This value is used to determine whether we should keep avoiding or not.
	DistanceFromHitLocation = FVector::Dist(GetActorLocation(), AvoidLocation);

	if (bShouldAvoid)
	{
		FVector Direction = AvoidLocation - Obstacle->GetActorLocation();

		// The avoidance force is a lateral direction from the obstacle
		if (Obstacle)
			Avoidance = FVector(-Direction.Y, Direction.X, Direction.Z).GetSafeNormal();

		#if !UE_BUILD_SHIPPING
		if (Debug.bShowAvoidanceForce)
			DrawDebugLine(World, AvoidLocation, AvoidLocation + Avoidance * 200.0f, FColor::Red, false, 4.0f, 0, 2.0f);
		#endif

		// Stop avoiding when the avoid location is far away from us.
		const bool bStopAvoiding = DistanceFromHitLocation > AvoidanceDistance/2.0f;
		if (bStopAvoiding)
		{
			// Stop avoiding and move back into the flock
			bShouldAvoid = false;
			bShouldRaycast = true;

			Avoidance = FVector(0.0f);
		}
	}
}

void ABoid::ApplyBehaviors()
{
	// Seek towards a goal
	if (Behaviors.GoalSeeking.bEnabled && Behaviors.GoalSeeking.Goal)
		GoalSeeking = SeekTowardsGoal();

	// Apply the three flocking rules when we can see other boids
	Alignment = Align(Boids);
	Cohesion = Cohere(Boids);
	Separation = Separate(Boids);

	if (bAllowObstacleAvoidance)
		CheckForObstacles();

	// Weights
	Alignment *= Behaviors.Alignment.Force;
	Cohesion *= Behaviors.Cohesion.Force;
	Separation *= Behaviors.Separation.Force;
	GoalSeeking *= Behaviors.GoalSeeking.Force;
	Avoidance *= Behaviors.Avoidance.Force;

	// Add all the forces together to produce the flocking vector
	ApplyForces();
}

void ABoid::ApplyForces()
{
	Flocking = Alignment + Cohesion + Separation;
	
	if (Behaviors.GoalSeeking.bEnabled)
	{
		Flocking += GoalSeeking;
	}
	else
	{
		// Return back if we've wandered off too far
		if (HasWanderedTooFar())
		{
			const FVector Direction = SpawnLocation - GetActorLocation();
			Flocking = Direction;
		}
	}

	if (bShouldAvoid)
		Flocking = Avoidance;
	
	// Apply the final flocking vector
	ApplyForce(Flocking);
}

void ABoid::OnBoidDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Boids.Num() == 0)
		SphereComponent->SetComponentTickEnabled(true);

	if (OtherActor->IsA(ABoid::StaticClass()))
	{
		Boids.AddUnique(Cast<ABoid>(OtherActor));
	}
}

void ABoid::OnBoidLost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ABoid::StaticClass()))
	{
		Boids.Remove(Cast<ABoid>(OtherActor));
	}

	if (Boids.Num() == 0)
		SphereComponent->SetComponentTickEnabled(false);
}

void ABoid::ApplyForce(const FVector& Force)
{
	// Calculate the new steering force if force is not an invalid number
	if (!Force.ContainsNaN())
	{
		const FVector Steering = Force + GetVelocity();

		BoidMovementComponent->AddInputVector(Force);
		
		CalculateRotation(Steering);
	}
}

FVector ABoid::Align(TArray<ABoid*>& Boids)
{
	FVector SumOfVectors = FVector(0.0f); // Used for calculating the average velocity
	const float Radius = PerceptionRadius; // The value of how far the boid see
	unsigned short Neighbors = 0; // Used to count how many neighbors (boids) there are within its perception radius

	// Loop through every single boid
	for (auto Other : Boids)
	{
		// Check their distances between each other
		const float Distance = FVector::Dist(GetActorLocation(), Other->GetActorLocation());

		// If they are within the boids perception
		if (Distance > 0 && Distance < Radius)
		{
			// Add up their velocities and add 1 to the amount of neighbors found
			SumOfVectors += Other->GetVelocity();
			Neighbors++;
		}
	}

	// Only calculate the steering force if there is at least 1 neighbor
	if (Neighbors > 0)
	{
		FVector AverageVelocity = SumOfVectors / Neighbors; // Get the average velocity vectors

		//AverageVelocity.Normalize();
		//AverageVelocity *= BoidMovementComponent->GetMaxSpeed();

		//FVector Steer = AverageVelocity + GetVelocity(); // Reynold's steering formula
		//
		//if (Steer.ContainsNaN())
		//	Steer = GetVelocity();
		//Velocity = AverageVelocity;
		return AverageVelocity;
	}

	return GetVelocity();
}

FVector ABoid::Cohere(TArray<ABoid*>& Boids)
{
	FVector SumOfVectors = FVector(0.0f); // Used for calculating the average location
	const float Radius = PerceptionRadius; // The value of how far the boid see
	unsigned short Neighbors = 0; // Used to count how many neighbors (boids) there are within its perception radius

	// Loop through every single boid
	for (auto Other : Boids)
	{
		// Check their distances between each other
		const float Distance = FVector::Dist(GetActorLocation(), Other->GetActorLocation());

		// If they are within the boids perception
		if (Distance > 0 && Distance < Radius)
		{
			// Add up their locations and add 1 to the amount of neighbors found
			SumOfVectors += Other->GetActorLocation();
			Neighbors++;
		}
	}

	// Only seek toward the average location if there is at least 1 neighbor
	if (Neighbors > 0)
	{
		FVector AverageLocation = SumOfVectors / Neighbors; // Get the average location

		//SumOfVectors.Normalize();
		//SumOfVectors *= BoidMovementComponent->GetMaxSpeed();

		return Seek(AverageLocation);
	}

	return GetVelocity();
}

FVector ABoid::Separate(TArray<ABoid*>& Boids)
{
	FVector SumOfVectors = FVector(0.0f); // Used for calculating the average location
	const float DesiredSeparation = PerceptionRadius - 100.0f; // The value of how far the boid see
	unsigned short Neighbors = 0; // Used to count how many neighbors (boids) there are within its perception radius

	// Loop through every single boid
	for (auto Other : Boids)
	{
		// Check their distances between each other
		const float Distance = FVector::Dist(GetActorLocation(), Other->GetActorLocation());

		// If they are within the boids perception
		if (Distance > 0 && Distance < DesiredSeparation)
		{
			// Calculate the difference between each other and add 1 to the amount of neighbors found
			FVector Difference = GetActorLocation() - Other->GetActorLocation();

			Difference.Normalize();
			Difference /= Distance;
			SumOfVectors += Difference;

			Neighbors++;
		}
	}

	// Only calculate the steering force if there is at least 1 neighbor
	if (Neighbors > 0)
	{
		FVector AverageLocation = SumOfVectors / Neighbors; // Get the average location

		AverageLocation.Normalize();
		AverageLocation *= BoidMovementComponent->GetMaxSpeed();

		// Reynold's steering formula
		FVector Steer = AverageLocation + GetVelocity();

		if (Steer.ContainsNaN())
			Steer = GetVelocity();

		return Steer;
	}

	return GetVelocity();
}

FVector ABoid::Seek(const FVector& Target)
{
	// Get the direction to the target
	const FVector Direction = Target - GetActorLocation();

	// Reynold's Steering formula
	FVector Steer = Direction + GetVelocity();

	if (Steer.ContainsNaN() || Steer.IsZero())
		Steer = GetVelocity();

	return Steer;
}

void ABoid::AssignMesh(UStaticMesh* NewMesh, TArray<UMaterialInterface*> Materials) const
{
	if (NewMesh)
	{
		StaticMeshComponent->SetStaticMesh(NewMesh);

		for (int32 i = 0; i < Materials.Num(); i++)
			StaticMeshComponent->SetMaterial(i, Materials[i]);
	}
}

void ABoid::SetMeshScale(const float NewScaleMultiplier) const
{
	StaticMeshComponent->SetWorldScale3D(FVector(NewScaleMultiplier));
}

void ABoid::SetMeshRotation(const FRotator NewRotation) const
{
	StaticMeshComponent->SetRelativeRotation(NewRotation);
}

float ABoid::GetPerceptionRadius() const
{
	return PerceptionRadius;
}

void ABoid::SetSpeed(const float NewSpeed) const
{
	BoidMovementComponent->MaxSpeed = NewSpeed;
}

void ABoid::SetWanderDistance(const float NewWanderDistance)
{
	WanderDistance = NewWanderDistance;
}

void ABoid::SetGoal(AGoal* NewGoal)
{
	Behaviors.GoalSeeking.Goal = NewGoal;
}

void ABoid::SetFlock(AFlock* NewFlock)
{
	OwnedBy = NewFlock;
}

void ABoid::EnablePlaneConstraint(const bool bValue)
{
	BoidMovementComponent->SetPlaneConstraintEnabled(bValue);
}

void ABoid::SetPlaneConstraintAxis(const EPlaneConstraintAxisSetting NewPlaneConstraintAxis)
{
	if (!BoidMovementComponent->bConstrainToPlane)
		BoidMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Custom);
	else
		BoidMovementComponent->SetPlaneConstraintAxisSetting(NewPlaneConstraintAxis);
}

void ABoid::CalculateRotation(FVector Steering)
{
	Rotation = Steering.Rotation();
}

void ABoid::UpdateRotation(const float DeltaTime)
{
	SetActorRotation(FMath::Lerp(GetActorRotation(), Rotation, TurnSpeed * DeltaTime));
}

void ABoid::EnableGoalSeeking(const bool bValue)
{
	Behaviors.GoalSeeking.bEnabled = bValue;
}

void ABoid::SetAlignmentForce(const float ForceMultiplier)
{
	Behaviors.Alignment.Force = ForceMultiplier;
}

void ABoid::SetCohesionForce(const float ForceMultiplier)
{
	Behaviors.Cohesion.Force = ForceMultiplier;
}

void ABoid::SetSeparationForce(const float ForceMultiplier)
{
	Behaviors.Separation.Force = ForceMultiplier;
}

void ABoid::SetGoalSeekingForce(const float ForceMultiplier)
{
	Behaviors.GoalSeeking.Force = ForceMultiplier;
}

void ABoid::SetAvoidanceForce(const float ForceMultiplier)
{
	Behaviors.Avoidance.Force = ForceMultiplier;
}

void ABoid::SetPerceptionRadius(const float NewPerceptionRadius)
{
	PerceptionRadius = NewPerceptionRadius;
}

void ABoid::SetShowAvoidanceForce(const bool NewValue)
{
	Debug.bShowAvoidanceForce = NewValue;
}

void ABoid::SetShowAvoidanceDistance(bool NewValue)
{
	Debug.bShowAvoidanceDistance = NewValue;
}

void ABoid::SetAllowAvoidanceBehavior(const bool NewValue)
{
	bAllowObstacleAvoidance = NewValue;
}

void ABoid::SetAvoidanceDistance(const float NewValue)
{
	AvoidanceDistance = NewValue;
}

void ABoid::SetTurnSpeed(const float NewTurnSpeed)
{
	TurnSpeed = NewTurnSpeed;
}

float ABoid::GetSpeed() const
{
	return BoidMovementComponent->GetMaxSpeed();
}

float ABoid::GetTurnSpeed() const
{
	return TurnSpeed;
}
