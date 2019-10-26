// Copyright Ali El Saleh, 2019

#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
	StaticMeshComponent->SetWorldScale3D(FVector(7.0f));

	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")));
	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'")));

	// Static mesh component settings
	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetMaterial(0, Material);
	}

	StaticMeshComponent->SetEnableGravity(false);
	StaticMeshComponent->bApplyImpulseOnDamage = false;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCollisionProfileName(FName("NoCollision"));
	StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

#if WITH_EDITOR
void AObstacle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == "bCastShadow")
	{
		StaticMeshComponent->SetCastShadow(bCastShadow);
	}
}
#endif

