// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeleportationComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UTeleportationComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTeleportationComponent();

	UFUNCTION(BlueprintPure, Category = "Teleportation Component")
		FVector FindLocationToTeleport(const FVector& Origin, float Radius, const FBox& InBox) const;

	UFUNCTION(BlueprintPure, Category = "Teleportation Component")
		FORCEINLINE float GetTeleportTime() const { return TeleportTime; }

protected:
	void BeginPlay() override;

	// Draw a sphere at the teleported location
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Debug")
		uint8 bShowTeleportedLocation : 1;

	// The amount of time (in seconds) we wait before we teleport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Debug", meta = (ClampMin = 0.0f))
		float TeleportTime = 1.0f;

private:
	AActor* Owner;
};
