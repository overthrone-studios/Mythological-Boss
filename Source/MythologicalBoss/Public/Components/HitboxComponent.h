// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "HitboxComponent.generated.h"

/**
 * A component that has certain settings tweaked to mimic a hitbox
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYTHOLOGICALBOSS_API UHitboxComponent final : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UHitboxComponent();

	// Returns the scalar value of this component
	UFUNCTION(BlueprintPure, Category = "Hitbox")
		FORCEINLINE float GetScalarValue() const { return Scalar; }

protected:
	// The value applied to a base value
	UPROPERTY(EditAnywhere, Category = "Hitbox", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
		float Scalar = 1.0f;
};
