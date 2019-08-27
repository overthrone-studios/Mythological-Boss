// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UFloatingComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFloatingComponent();

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The amount of units to offset by on an axis
	UPROPERTY(EditAnywhere, Category = "Floating Movement")
		FVector Offset = FVector(0.0f, 0.0f, 50.0f);

	// The movement speed while floating (1 = normal speed | >1 = fast | <1 = slow)
	UPROPERTY(EditAnywhere, Category = "Floating Movement", meta = (ClampMin = 0.0f))
		float Speed = 1;

private:
	// Tracks the amount of movement overtime
	float RunningTime = 0.0f;

	AActor* Owner;
};
