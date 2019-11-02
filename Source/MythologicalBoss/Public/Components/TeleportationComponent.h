// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "TeleportationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginReappearSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisappearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReappearedSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UTeleportationComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTeleportationComponent();

	FOnBeginReappearSignature OnBeginReappear;
	FOnBeginReappearSignature OnBeginDisappear;

	FOnDisappearedSignature OnDisappeared;
	FOnReappearedSignature OnReappeared;

	void Disappear();
	void Reappear();

	UFUNCTION(BlueprintPure, Category = "Teleportation Component")
		class UMaterialInstanceDynamic* GetDissolveMaterial() const { return MID_Dissolve; }

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
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void UpdateDissolve();

	UFUNCTION()
		void FinishDissolve();

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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Teleport")
		class UCurveFloat* DissolveCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Teleport", meta = (ClampMin = 0.0f))
		float DissolveSpeed = 1.0f;

private:
	class UMaterialInterface* OriginalMaterial;

	class USkeletalMeshComponent* SKMComponent;
	class UMaterialInstanceDynamic* MID_Dissolve;

	FTimeline TL_Dissolve;

	FTimerHandle TH_Cooldown;

	FTimerManager* TimerManager;

	class ACharacter* Owner;

	uint8 bWasReversing : 1;
};
