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

	UFUNCTION(BlueprintCallable, Category = "Box")
		FORCEINLINE FBox GetBoundingBox() const;

protected:
	void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
		class UBoxComponent* BoxComponent;
};
