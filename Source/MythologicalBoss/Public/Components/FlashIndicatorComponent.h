// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "FlashIndicatorComponent.generated.h"

/**
 * Flashes whenever an attack is about to happen
 */
UCLASS()
class MYTHOLOGICALBOSS_API UFlashIndicatorComponent final : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UFlashIndicatorComponent();

	UFUNCTION(BlueprintCallable, Category = "Flash Indicator")
		void Flash(const FLinearColor& FlashColor);
	
protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void GrowFlash();

	UFUNCTION()
		void FinishFlash();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Flash Settings")
		class UMaterialInterface* FlashMaterial;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Flash Settings")
		class UCurveFloat* FlashCurve;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Flash Settings", meta = (ClampMin = 0.0f))
		float FlashSpeed = 2.0f;

private:
	class UMaterialInstanceDynamic* MID_FlashIndicator;

	FLinearColor CurrentColor;
	FTimeline FlashTimeline;
};
