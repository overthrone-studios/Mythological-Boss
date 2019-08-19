// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChargeAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UChargeAttackComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChargeAttackComponent();

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

	// The amount of charge we gain after a successful hit
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack", meta = (ClampMin = 0.0f))
		float ChargeGainPerHit = 10.0f;

	// The amount of charge we loss after we've taken damage
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack", meta = (ClampMin = 0.0f))
		float ChargeLossPerHit = 20.0f;

	// Should we reset the charge meter when we've taken damage?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack")
		uint8 bResetChargeAfterMaxHits : 1;

	// The number of hits we can take before our charge is reset
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack", meta = (EditCondition = "bResetChargeAfterMaxHits"))
		uint8 MaxHitsForChargeReset = 1;

	// Should we lose charge overtime?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack")
		uint8 bLoseChargeOvertime : 1;

	// The amount of time (in seconds) to delay, before starting to lose charge
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack", meta = (EditCondition = "bLoseChargeOvertime", ClampMin = 0.0f, ClampMax = 60.0f))
		float DelayBeforeChargeLoss = 3.0f;

	// The speed of the charge loss (How fast or slow do we lose charge?)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack", meta = (EditCondition = "bLoseChargeOvertime", ClampMin = 0.0f))
		float ChargeLossRate = 30.0f;

	// Should we use a smooth charge meter bar?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Charge Attack")
		uint8 bSmoothBar : 1;

	// The actor's previous charge
	UPROPERTY(BlueprintReadOnly, Category = "Charge Attack")
		float PreviousCharge;

	// The actor's smoothed charge
	UPROPERTY(BlueprintReadOnly, Category = "Charge Attack")
		float NewCharge;

	// The actor that owns this component
	UPROPERTY(BlueprintReadOnly)
		AActor* Owner;
};
