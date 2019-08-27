// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotatingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API URotatingComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	URotatingComponent();

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// The rate of rotation (in degrees) per second, the actor rotates on specified axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector RotationRate = FVector(0.0f, 0.0f, 100.0f);

private:
	AActor* Owner;
};
