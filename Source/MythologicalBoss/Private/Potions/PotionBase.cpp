// Copyright Overthrone Studios 2019

#include "Potions/PotionBase.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/FloatingComponent.h"

#include "Engine/StaticMesh.h"

#include "Log.h"

APotionBase::APotionBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh Component"));
	FloatingComponent = CreateDefaultSubobject<UFloatingComponent>(FName("Floating Component"));
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("Sphere Collision Component"));

	RootComponent = StaticMeshComponent; // Set the mesh as the root

	// Load a primitive cube as the default potion mesh
	PotionMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")));

	// Static mesh component default setup
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
	StaticMeshComponent->SetStaticMesh(PotionMesh);
	StaticMeshComponent->SetWorldScale3D(FVector(0.5f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Sphere component setup
	SphereCollisionComponent->SetupAttachment(RootComponent);
	SphereCollisionComponent->SetRelativeLocation(FVector(0.0f));
	SphereCollisionComponent->SetSphereRadius(100.0f);
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APotionBase::Pickup);
}

void APotionBase::Pickup_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(0 && "You must implement Pickup_Implementation()");
}
