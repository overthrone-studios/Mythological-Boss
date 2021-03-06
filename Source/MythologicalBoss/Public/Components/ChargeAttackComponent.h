// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChargeAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChargeFullSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChargeEmptySignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UChargeAttackComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChargeAttackComponent();

	void InitChargeAttackEconomyValues(const class UYlvaDifficultyData& DifficultyData);

	FOnChargeFullSignature OnChargeFilled;
	FOnChargeEmptySignature OnChargeEmpty;

	// Delay charge loss by the DelayBeforeChargeLoss property
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		void DelayChargeLoss();

	// Returns true if the charge loss timer is inactive
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		bool IsLosingCharge();

	// Return the actor's maximum hits
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE uint8 GetMaxHits() const { return MaxHitsForChargeReset; }

	// Return the actor's maximum hits
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetChargeHoldFrames() const { return ChargeHoldTime; }

	// Return the actor's health gain on charge attack
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetHealthGainOnChargeAttack() const { return HealthGainOnChargeAttack; }

	// Return the actor's default charge value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetMaxCharge() const { return MaxCharge; }

	// Return the actor's current charge value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetCurrentCharge() const { return Charge; }

	// Return the actor's default charge value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetMaxChargeHoldTime() const { return MaxChargeHoldTime; }

	// Return the actor's previous charge value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetPreviousCharge() const { return PreviousCharge; }

	// Return the actor's smoothed charge value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetSmoothedCharge() const { return SmoothedCharge; }
	
	// Returns true if our current charge value is >= to our maximum charge value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE bool IsChargeFull() const { return Charge >= MaxCharge; }

	// Returns true if our current charge value is <= 0
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE bool IsChargeEmpty() const { return Charge <= 0.0f; }

	// Returns true if we can lose charge overtime
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE bool CanLoseChargeOvertime() const { return bLoseChargeOvertime; }

	// Returns true if we want to reset charge after max hits
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE bool WantsResetAfterMaxHits() const { return bResetChargeAfterMaxHits; }

	// Return the charge gain per hit value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetChargeGainForLight() const { return ChargeGainPerLightHit; }

	// Return the charge gain per hit value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetChargeGainForHeavy() const { return ChargeGainPerHeavyHit; }

	// Return the charge loss per hit value
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetChargeLoss() const { return ChargeLossPerHit; }

	// Return the charge loss rate
	UFUNCTION(BlueprintPure, Category = "Charge Attack")
		FORCEINLINE float GetChargeLossRate() const { return ChargeLossRate; }

	// Set a new current charge value
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		void SetCharge(float NewChargeAmount);

	// Set the new charge value
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		void SetSmoothedCharge(float Value);

	// Add charge by an amount
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		void IncreaseCharge(float Amount);

	// Subtract charge by an amount
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		void DecreaseCharge(float Amount);

	// Resets charge back to default
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		void ResetCharge();

	// Gets the material for the full charge meter
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		class UMaterialInterface* GetFullChargeMaterial() const { return FullChargeMaterial; }

protected:
	void BeginPlay() override;

	// Initialize the actor's charge attack values
	UFUNCTION(BlueprintCallable, Category = "Charge Attack")
		void InitChargeAttack();

	// The actor's current charge
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Charge Attack", meta = (ClampMin = 0.0f))
		float Charge = 100.0f;

	// The actor's maximum charge
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack", meta = (ClampMin = 0.0f))
		float MaxCharge = 100.0f;

	// The amount of charge we gain after a successful light attack hit
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (ClampMin = 0.0f))
		float ChargeGainPerLightHit = 10.0f;

	// The amount of charge we gain after a successful heavy attack hit
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (ClampMin = 0.0f))
		float ChargeGainPerHeavyHit = 20.0f;

	// The amount of charge we lose after we've taken damage
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (ClampMin = 0.0f))
		float ChargeLossPerHit = 20.0f;

	// The amount of health we gain when we've successfully landed a charge attack
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (ClampMin = 0.0f))
		float HealthGainOnChargeAttack = 20.0f;

	// The amount of time (in seconds) required to hold the charge for, before releasing it
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (ClampMin = 0))
		float ChargeHoldTime = 1.0f;

	// The maximum amount of time (in seconds) we can hold the charge attack for
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (ClampMin = 0.0f))
		float MaxChargeHoldTime = 6.0f;

	// Should we reset the charge meter when we've taken damage?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy")
		uint8 bResetChargeAfterMaxHits : 1;

	// The number of hits we can take before our charge is reset
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (EditCondition = "bResetChargeAfterMaxHits"))
		uint8 MaxHitsForChargeReset = 1;

	// Should we lose charge overtime?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy")
		uint8 bLoseChargeOvertime : 1;

	// The amount of time (in seconds) to delay, before starting to lose charge
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (EditCondition = "bLoseChargeOvertime", ClampMin = 0.0f, ClampMax = 60.0f))
		float DelayBeforeChargeLoss = 3.0f;

	// The speed of the charge loss (How fast or slow do we lose charge?)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack Economy", meta = (EditCondition = "bLoseChargeOvertime", ClampMin = 0.0f))
		float ChargeLossRate = 30.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Charge Attack")
		class UMaterialInterface* FullChargeMaterial;

	// The actor's previous charge
	UPROPERTY(BlueprintReadOnly, Category = "Charge Attack")
		float PreviousCharge;

	// The actor's smoothed charge
	UPROPERTY(BlueprintReadOnly, Category = "Charge Attack")
		float SmoothedCharge;

	// The actor that owns this component
	UPROPERTY(BlueprintReadOnly)
		AActor* Owner;

	FTimerHandle ChargeLossTimerHandle;
};
