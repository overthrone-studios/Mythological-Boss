// Copyright Ali El Saleh, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS(HideCategories = ("Input", "Actor", "LOD", "Cooking"))
class FLOCKINGBEHAVIOURSYSTEM_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstacle();

protected:
	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	UPROPERTY()
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category = "Rendering")
		bool bCastShadow = true;

private:
	UStaticMesh* StaticMesh;
	UMaterialInterface* Material;
};
