// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoundingBox.generated.h"

UCLASS(HideCategories = ("Input", "Rendering", "Actor", "LOD", "Cooking"))
class MYTHOLOGICALBOSS_API ABoundingBox final : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoundingBox();

	UFUNCTION(BlueprintPure, Category = "Box")
		FBox GetBoundingBox() const;

	UFUNCTION(BlueprintPure, Category = "Box")
		TArray<FVector> GetNodes() const { return Nodes; }

protected:
	void BeginPlay() override;

#if !UE_BUILD_SHIPPING
	void Tick(float DeltaSeconds) override;
#endif

#if WITH_EDITOR
	void OnConstruction(const FTransform& Transform) override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	bool ShouldTickIfViewportsOnly() const override;
#endif

	void CreateNodes();
	void ClearNodes();

	UPROPERTY(EditInstanceOnly)
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditInstanceOnly, Category = "Nodes")
		uint16 Rows = 7;

	UPROPERTY(EditInstanceOnly, Category = "Nodes")
		uint16 Columns = 7;

	UPROPERTY(EditInstanceOnly, Category = "Nodes")
		uint16 NodeSpacing = 3;

	UPROPERTY(EditInstanceOnly, Category = "Nodes")
		float NodeZHeight = 130.0f;

	TArray<FVector> Nodes;

private:
	FVector MinBoxExtent, MaxBoxExtent;
};
