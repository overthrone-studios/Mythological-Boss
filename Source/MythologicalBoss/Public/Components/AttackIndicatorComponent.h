// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "AttackIndicatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTHOLOGICALBOSS_API UAttackIndicatorComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttackIndicatorComponent();

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

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Flash Settings", meta = (ClampMin = 0))
		uint8 MaterialIndex = 0;

private:
	class ACharacter* Owner;

	class UMaterialInstanceDynamic* MID_FlashIndicator;

	FLinearColor CurrentColor;
	FTimeline FlashTimeline;
};
