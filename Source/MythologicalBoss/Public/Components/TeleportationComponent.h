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
		FVector FindLocationToTeleport(const FVector& Origin, float Radius, const class ABoundingBox* InPlayArea) const;

	UFUNCTION(BlueprintPure, Category = "Teleportation Component")
		FORCEINLINE float GetTeleportTime() const { return TeleportTime; }

	UFUNCTION(BlueprintCallable, Category = "Teleportation Component")
		void GenerateTeleportTime();

	UFUNCTION(BlueprintCallable, Category = "Teleportation Component")
		void StartCooldown();

	UFUNCTION(BlueprintPure, Category = "Teleportation Component")
		bool IsCoolingDown();

protected:
	void BeginPlay() override;

	// Draw a sphere at the teleported location
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Teleport Debug")
		uint8 bShowTeleportedLocation : 1;

	// Log the teleport time (in seconds) to the viewport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Teleport Debug")
		uint8 bLogTeleportTime : 1;

	// The amount of time (in seconds) we wait before we teleport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Teleport", meta = (ClampMin = 0.0f))
		float TeleportTime = 1.0f;

	// Adds a range range to TeleportTime
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Teleport", meta = (ClampMin = 0.0f))
		float RandomDeviation = 0.2f;

	// The amount of time (in seconds) to cooldown before accepting a teleport
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Teleport", meta = (ClampMin = 0.0f))
		float CooldownTime = 1.0f;

private:
	FTimerHandle TH_Cooldown;

	FTimerManager* TimerManager;

	AActor* Owner;
};
