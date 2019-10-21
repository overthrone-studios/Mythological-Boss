// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "OverthroneStructs.h"
#include "PlayerCameraComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnableLockOnSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableLockOnSignature);

USTRUCT(BlueprintType)
struct FCameraShakes_Player
{
	GENERATED_BODY()

	// The camera shake to play when we are blocking the boss's attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Damaged;

	// The camera shake to play when we are blocking the boss's attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData ShieldHit;

	// The camera shake to play while idleling
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Idle;

	// The camera shake to play while walking
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Walk;

	// The camera shake to play while running
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Run;

	// The camera shake to play while charging our attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData Charge;

	// The camera shake to play when we release our charge attack
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		FCameraShakeData ChargeEnd;
};

/**
 * A custom camera that can be attack to a player character
 */
UCLASS()
class MYTHOLOGICALBOSS_API UPlayerCameraComponent final : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCameraComponent();

	UPROPERTY(BlueprintAssignable, Category = "Camera | Lock-On")
		FOnEnableLockOnSignature OnEnableLockOn;

	UPROPERTY(BlueprintAssignable, Category = "Camera | Lock-On")
		FOnDisableLockOnSignature OnDisableLockOn;

	UFUNCTION(BlueprintPure, Category = "Camera | General")
		float GetMaxPitch() const { return CameraPitchMax; }

	UFUNCTION(BlueprintPure, Category = "Camera | General")
		float GetMinPitch() const { return CameraPitchMin; }

	UFUNCTION(BlueprintPure, Category = "Camera | Lock-On")
		float GetMaxLockOnDistance() const { return MaxLockOnDistance; }

	UFUNCTION(BlueprintPure, Category = "Camera | Lock-On")
		float GetMaxLockOnPitch() const { return LockOnPitchMax; }
	
	UFUNCTION(BlueprintPure, Category = "Camera | Lock-On")
		float GetMinLockOnPitch() const { return LockOnPitchMin; }
		
	UFUNCTION(BlueprintPure, Category = "Camera | Lock-On")
		float GetLockOnRotationSpeed() const { return LockOnRotationSpeed; }

	UFUNCTION(BlueprintPure, Category = "Camera | Lock-On")
		const FCameraShakes_Player& GetShakes() const { return Shakes; }

	// Is camera currently lock-on?
	UFUNCTION(BlueprintPure, Category = "Camera | Lock-On")
		bool IsLockedOn() const { return bIsLockedOn; }

	UFUNCTION(BlueprintCallable, Category = "Camera | Lock-On")
		void ToggleLockOn();
	
	UFUNCTION(BlueprintCallable, Category = "Camera | Lock-On")
		void EnableLockOn();
	
	UFUNCTION(BlueprintCallable, Category = "Camera | Lock-On")
		void DisableLockOn();

	void OscillateVignette();
	void StopOscillatingVignette();

	void DesaturateScreen();
	void ResaturateScreen();

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void UpdateSaturation();

	UFUNCTION()
		void UpdateVignette();

	// The minimum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera | General", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMin = 30.0f;

	// The maximum pitch rotation value (in degrees) the camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera | General", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float CameraPitchMax = 50.0f;

	// The maximum distance that we are allowed to lock on
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera | Lock-On", meta = (ClampMin = 0.0f))
		float MaxLockOnDistance = 3000.0f;

	// The minimum pitch rotation value (in degrees) the lock on camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera | Lock-On", meta = (ClampMin = "-90.0", ClampMax = 90.0f))
		float LockOnPitchMin = 5.0f;

	// The maximum pitch rotation value (in degrees) the lock on camera can rotate
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera | Lock-On", meta = (ClampMin = 0.0f, ClampMax = 90.0f))
		float LockOnPitchMax = 30.0f;

	// The rotation speed of the lock on camera (1 = Slow, 10+ = Fast)
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera | Lock-On", meta = (ClampMin = 0.0f))
		float LockOnRotationSpeed = 10.0f;

	// Ylva's camera shake settings
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Ylva Camera | Camera Shakes")
		FCameraShakes_Player Shakes;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Saturation")
		class UCurveFloat* SaturationCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Saturation", meta = (ClampMin = 0.0f))
		float SaturationSpeed = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Vignette")
		class UCurveFloat* VignetteCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Vignette", meta = (ClampMin = 0.0f))
		float VignetteSpeed = 1.0f;

private:
	APlayerController* PlayerController;

	uint8 bIsLockedOn : 1;

	FTimeline TL_ScreenSaturation;
	FTimeline TL_Vignette;
};
