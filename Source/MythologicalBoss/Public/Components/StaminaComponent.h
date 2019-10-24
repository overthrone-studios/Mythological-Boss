// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UStaminaComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent();

	// Delay stamina regeneration by the regeneration delay
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void DelayRegeneration();

	// Returns true if the regen timer is not active
	UFUNCTION(BlueprintPure, Category = "Stamina")
		bool IsRegenFinished();

	// Returns true if the delay timer is not active
	UFUNCTION(BlueprintPure, Category = "Stamina")
		bool IsDelayFinished();

	// Returns the delay timer handle
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FTimerHandle& GetDelayTimerHandle() { return DelayTimerHandle; }

	// Return the actor's default stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetDefaultStamina() const { return DefaultStamina; }

	// Return the actor's current stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetCurrentStamina() const { return Stamina; }

	// Return the actor's previous stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetPreviousStamina() const { return PreviousStamina; }

	// Return the actor's new stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetSmoothedStamina() const { return SmoothedStamina; }

	// Returns true if smooth bar is enabled
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetLowStaminaThreshold() const { return LowStaminaThreshold; }

	// Return the delay value when using smooth bar
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetDecreaseDelay() const { return Delay; }

	// Returns true if we have enough stamina for light attacking
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE bool HasEnoughForLightAttack() const { return Stamina > LightAttack; }

	// Returns true if we have enough stamina for light attacking
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE bool HasEnoughForHeavyAttack() const { return Stamina > HeavyAttack; }

	// Returns true if we have enough stamina for light attacking
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE bool HasEnoughForDash() const { return Stamina > Dash; }

	// Returns true if we have enough stamina for dash attacking
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE bool HasEnoughForDashAttack() const { return Stamina > DashAttack; }

	// Returns true if we don't have stamina (Stamina <= 0)
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE bool IsStaminaEmpty() const { return Stamina <= 0.0f; }

	// Returns true if we are low on stamina
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE bool IsLowStamina() const { return Stamina <= DefaultStamina * LowStaminaThreshold; }

	// Returns true if we have stamina (Stamina > 0)
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE bool HasStamina() const { return Stamina > 0.0f; }

	// Return the regeneration rate value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetRegenRate() const { return RegenerationRate; }

	// Return the regeneration delay value (in seconds)
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetRegenDelay() const { return RegenerationDelay; }

	// Return the light attack stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetLightAttackValue() const { return LightAttack; }

	// Return the heavy attack stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetHeavyAttackValue() const { return HeavyAttack; }

	// Return the dash stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetDashValue() const { return Dash; }

	// Return the dash stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetChargeAttackValue() const { return ChargeAttack; }

	// Return the dash attack stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetDashAttackValue() const { return DashAttack; }

	// Return the run stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetRunValue() const { return Run; }

	// Return the shield hit stamina value
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FORCEINLINE float GetShieldHitValue() const { return ShieldHit; }

	// Set a new current stamina value
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void SetStamina(float NewStaminaAmount);

	// Set the new stamina value
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void SetSmoothedStamina(float Value);

	// Set the previous stamina value
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void SetPreviousStamina(float Value);

	// Update the previous stamina value to the smoothed stamina value
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void UpdatePreviousStamina();

	// Add stamina by an amount
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void IncreaseStamina(float Amount);

	// Subtract stamina by an amount
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void DecreaseStamina(float Amount);

	// Resets stamina back to default
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void ResetStamina();

protected:
	void BeginPlay() override;

	// Initialize the actor's stamina values
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void InitStamina();

	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	// The actor's current stamina
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = 0.0f))
		float Stamina = 100.0f;

	// The actor's default health
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = 0.0f))
		float DefaultStamina = 100.0f;

	// The actor's low stamina point
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float LowStaminaThreshold = 0.25f;

	// The amount of time (in seconds) we should wait before decreasing stamina smoothly
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
		float Delay = 0.5f;

	// The actor's previous health before being damaged
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float PreviousStamina;

	// The actor's smoothed stamina
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float SmoothedStamina;

	// The rate of stamina regeneration (How fast do we replenish stamina)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float RegenerationRate = 200.0f;

	// The amount of time (in seconds) we should wait before regenerating stamina
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f, ClampMax = 60.0f))
		float RegenerationDelay = 1.0f;

	// The stamina value to subtract when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float LightAttack = 10.0f;

	// The stamina value to subtract when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float HeavyAttack = 30.0f;

	// The stamina value to subtract when dashing
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float Dash = 40.0f;

	// The stamina value to subtract when dash attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float DashAttack = 150.0f;

	// The stamina value to subtract when charge attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float ChargeAttack = 200.0f;

	// The stamina value to subtract while running
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float Run = 100.0f;

	// The stamina value to subtract when being hit while blocking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stamina Economy", meta = (ClampMin = 0.0f))
		float ShieldHit = 300.0f;

private:
	AActor* Owner;

	FTimerHandle RegenTimerHandle;
	FTimerHandle DelayTimerHandle;
};
