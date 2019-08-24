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

	UFUNCTION(BlueprintCallable, Category = "Teleportation Component")
		FVector FindLocationToTeleport(const FVector& Origin, float Radius, const FBox& InBox) const;

protected:
	void BeginPlay() override;

	// Draw a sphere at the teleported location
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Debug")
		uint8 bShowTeleportedLocation : 1;

private:
	AActor* Owner;
};
