// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UHealthComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	// Returns true if the delay timer is not active
	UFUNCTION(BlueprintPure, Category = "Health")
		bool IsDelayFinished();

	// Returns the delay timer handle
	UFUNCTION(BlueprintPure, Category = "Health")
		FTimerHandle& GetDelayTimerHandle() { return DelayTimerHandle; }

	// Return the actor's default health value
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetDefaultHealth() const { return DefaultHealth; }

	// Return the actor's current health value
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return Health; }

	// Return the actor's previous health value
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetPreviousHealth() const { return PreviousHealth; }

	// Return the actor's new health value
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetSmoothedHealth() const { return NewHealth; }

	// Return the actor's low health threshold value as percentage
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetLowHealthThreshold() const { return LowHealthThreshold; }
	
	// Returns true if smooth bar is enabled
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE bool IsUsingSmoothBar() const { return bSmoothBar; }

	// Returns true if the actor's health is less than or equal to the low health threshold
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE bool IsLowHealth() const { return Health <= DefaultHealth * LowHealthThreshold; }

	// Return the delay value when using smooth bar
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetDecreaseDelay() const { return Delay; }
	
	// Set a new current health value
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(float NewHealthAmount);

	// Set the new health value
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetSmoothedHealth(float Value);

	// Add health by an amount
	UFUNCTION(BlueprintCallable, Category = "Health")
		void IncreaseHealth(float Amount);

	// Subtract health by an amount
	UFUNCTION(BlueprintCallable, Category = "Health")
		void DecreaseHealth(float Amount);

	// Resets health back to default
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ResetHealth();

protected:
	void BeginPlay() override;

	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	// Initialize the actor's health values
	UFUNCTION(BlueprintCallable, Category = "Health")
		void InitHealth();

	// Called when the actor has taken damage
	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// The actor's current health
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.0f))
		float Health = 100.0f;

	// The actor's default health
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.0f))
		float DefaultHealth = 100.0f;

	// The actor's low health threshold as percentage
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float LowHealthThreshold = 0.25f;

	// Should we use a smooth health bar?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Health")
		uint8 bSmoothBar : 1;

	// The amount of time (in seconds) we should wait before decreasing health smoothly
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Health", meta = (EditCondition = "bSmoothBar", ClampMin = 0.0f, ClampMax = 10.0f))
		float Delay = 0.5f;

	// The actor's previous health before being damaged
	UPROPERTY(BlueprintReadOnly, Category = "Health")
		float PreviousHealth;

	// The actor's updated health when bSmoothBar is enabled
	UPROPERTY(BlueprintReadOnly, Category = "Health")
		float NewHealth;

	// The actor that owns this component
	UPROPERTY(BlueprintReadOnly)
		AActor* Owner;

private:
	FTimerHandle DelayTimerHandle;
};
