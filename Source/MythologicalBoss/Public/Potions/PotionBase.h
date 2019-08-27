// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PotionBase.generated.h"

UCLASS(Abstract)
class MYTHOLOGICALBOSS_API APotionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APotionBase();

	// Returns this potions value
	UFUNCTION(BlueprintPure, Category = "Potion")
		FORCEINLINE int32 GetPotionValue() const { return Value; }

	// Returns the pickup sound
	UFUNCTION(BlueprintPure, Category = "Potion")
		FORCEINLINE class USoundBase* GetPickupSound() const { return PickupSound; }

protected:
	// Called when an actor has overlapped with the sphere collision component
	UFUNCTION(BlueprintNativeEvent)
		void Pickup(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void Pickup_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// The component that represents the potion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion")
		class UStaticMeshComponent* StaticMeshComponent;

	// The component that detects if an actor has overlapped with it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion")
		class USphereComponent* SphereCollisionComponent;

	// A component that makes this actor float in a sin wave fashion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion")
		class UFloatingComponent* FloatingComponent;

	// A component that makes this actor rotate with a given rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion")
		class URotatingComponent* RotatingComponent;

	// The value of this potion
	UPROPERTY(EditInstanceOnly, Category = "Potion", meta = (ClampMin = 0))
		int32 Value = 10;

	// The sound to play on potion pick up
	UPROPERTY(EditInstanceOnly, Category = "Potion")
		class USoundBase* PickupSound;

	// The mesh representing the potion
	class UStaticMesh* PotionMesh;
};
