// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UDashComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDashComponent();

	// Returns the forward dash animation montage
	UFUNCTION(BlueprintPure, Category = "Dash")
		FORCEINLINE float GetCooldownTime() const { return DashCooldown; }

	// Returns true if the dash cooldown timer is active
	UFUNCTION(BlueprintPure, Category = "Dash")
		bool IsCooldownActive();

	// Returns the dash cooldown time remaining, in seconds
	UFUNCTION(BlueprintPure, Category = "Dash")
		float GetCooldownTimeRemaining();

	// Returns true if the dash cooldown timer is active
	UFUNCTION(BlueprintCallable, Category = "Dash")
		void StartCooldown();

protected:
	void BeginPlay() override;

	// The amount of time (in seconds) until we can dash again
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 60.0f))
		float DashCooldown = 1.0f;

private:
	AActor* Owner;

	FTimerHandle DashCooldownTimer;
};
