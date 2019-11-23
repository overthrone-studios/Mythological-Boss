// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "MordathSKMComponentBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginReappearSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisappearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReappearedSignature);

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMordathSKMComponentBase : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UMordathSKMComponentBase();

	UPROPERTY(BlueprintAssignable, Category = "Mordath SKM Component")
		FOnBeginReappearSignature OnBeginReappear;

	UPROPERTY(BlueprintAssignable, Category = "Mordath SKM Component")
		FOnBeginReappearSignature OnBeginDisappear;

	UPROPERTY(BlueprintAssignable, Category = "Mordath SKM Component")
		FOnDisappearedSignature OnDisappeared;

	UPROPERTY(BlueprintAssignable, Category = "Mordath SKM Component")
		FOnReappearedSignature OnReappeared;

	UFUNCTION(BlueprintCallable, Category = "Mordath SKM Component")
		void Reappear();

	UFUNCTION(BlueprintCallable, Category = "Mordath SKM Component")
		void Disappear();

	UFUNCTION(BlueprintCallable, Category = "Mordath SKM Component")
		void UpdateVectorParameter(const FName& ParameterName, const FLinearColor& Vector);
	
	UFUNCTION(BlueprintCallable, Category = "Mordath SKM Component")
		void UpdateScalarParameter(const FName& ParameterName, float Scalar);

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void UpdateDissolve();

	UFUNCTION()
		void FinishDissolve();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dissolve")
		class UCurveFloat* DissolveCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dissolve", meta = (ClampMin = 0.0f))
		float DissolveSpeed = 1.0f;

	class UMaterialInterface* OriginalMaterial;
	class UMaterialInstanceDynamic* MID_OriginalMaterial;

	uint8 bWasReversing : 1;

	FTimeline TL_Dissolve;

	class ACharacter* Owner;
};
