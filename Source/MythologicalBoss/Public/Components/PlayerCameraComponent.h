// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "PlayerCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UPlayerCameraComponent final : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCameraComponent();

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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Saturation")
		class UCurveFloat* SaturationCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Saturation", meta = (ClampMin = 0.0f))
		float SaturationSpeed = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Vignette")
		class UCurveFloat* VignetteCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect | Vignette", meta = (ClampMin = 0.0f))
		float VignetteSpeed = 1.0f;

private:
	FTimeline TL_ScreenSaturation;
	FTimeline TL_Vignette;
};
