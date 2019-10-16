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

	void DesaturateScreen();
	void ResaturateScreen();

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void UpdateSaturation();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect")
		class UCurveFloat* SaturationCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Low Health Effect", meta = (ClampMin = 0.0f))
		float Speed = 1.0f;

private:
	FTimeline TL_ScreenSaturation;
};
