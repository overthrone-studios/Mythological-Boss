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

	// Returns true if the dash cooldown timer is paused
	UFUNCTION(BlueprintPure, Category = "Dash")
		bool IsCooldownPaused();

	// Returns the dash cooldown time remaining, in seconds
	UFUNCTION(BlueprintPure, Category = "Dash")
		float GetCooldownTimeRemaining();

	// Start the dash cooldown timer
	UFUNCTION(BlueprintCallable, Category = "Dash")
		void StartCooldown();

	// Pause the dash cooldown timer
	UFUNCTION(BlueprintCallable, Category = "Dash")
		void PauseCooldown();

	// Resume the dash cooldown timer
	UFUNCTION(BlueprintCallable, Category = "Dash")
		void UnPauseCooldown();

	// Stop the dash cooldown timer
	UFUNCTION(BlueprintCallable, Category = "Dash")
		void StopCooldown();

	UFUNCTION(BlueprintPure, Category = "Dash")
		class UAnimMontage* GetDashForwardAnim() const { return DashForwardAnim; }

	UFUNCTION(BlueprintPure, Category = "Dash")
		class UAnimMontage* GetDashBackwardAnim() const { return DashBackwardAnim; }

	UFUNCTION(BlueprintPure, Category = "Dash")
		class UAnimMontage* GetDashLeftAnim() const { return DashLeftAnim; }

	UFUNCTION(BlueprintPure, Category = "Dash")
		class UAnimMontage* GetDashRightAnim() const { return DashRightAnim; }

protected:
	void BeginPlay() override;

	// The amount of time (in seconds) until we can dash again
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 60.0f))
		float DashCooldown = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UAnimMontage* DashForwardAnim;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UAnimMontage* DashBackwardAnim;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UAnimMontage* DashLeftAnim;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		class UAnimMontage* DashRightAnim;

private:
	AActor* Owner;

	FTimerHandle DashCooldownTimer;
};
