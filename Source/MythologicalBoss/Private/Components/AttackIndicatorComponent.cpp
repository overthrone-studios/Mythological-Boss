// Copyright Overthrone Studios 2019

#include "AttackIndicatorComponent.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Curves/CurveFloat.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"

#include "Kismet/KismetMaterialLibrary.h"

#include "Log.h"

UAttackIndicatorComponent::UAttackIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	// Initialize flash indicator
	if (FlashMaterial)
	{
		MID_FlashIndicator = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, FlashMaterial);
		MID_FlashIndicator->SetScalarParameterValue("Opacity", 1.0f);
		Owner->GetMesh()->SetMaterial(MaterialIndex, MID_FlashIndicator);
	}
	else
		ULog::Error(GetName() + ": There is no flash material!", true);

	// Flash Timeline Initialization
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, "GrowFlash");
	TimelineFinishedCallback.BindUFunction(this, "FinishFlash");

	if (FlashCurve)
	{
		FlashTimeline.SetLooping(false);
		FlashTimeline.SetPlayRate(1.0f);
		FlashTimeline.AddInterpFloat(FlashCurve, TimelineCallback);
		FlashTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		FlashTimeline.SetTimelineLength(FlashCurve->FloatCurve.Keys[FlashCurve->FloatCurve.Keys.Num() - 1].Time);
		FlashTimeline.SetTimelineLengthMode(TL_TimelineLength);
	}
	else
	{
		ULog::DebugMessage(ERROR, FString(GetName() + ": Failed to initialize timeline. A curve float asset is missing!"), true);
	}
}

void UAttackIndicatorComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FlashTimeline.TickTimeline(DeltaTime);
}

void UAttackIndicatorComponent::Flash(const FLinearColor& FlashColor)
{
	MID_FlashIndicator->SetVectorParameterValue("Color", FlashColor);
	MID_FlashIndicator->SetScalarParameterValue("Opacity", 0.0f);

	FlashTimeline.PlayFromStart();
}

void UAttackIndicatorComponent::GrowFlash()
{
	const float& Time = FlashTimeline.GetPlaybackPosition();
	const float& Alpha = FlashCurve->GetFloatValue(Time);

	MID_FlashIndicator->SetScalarParameterValue("Opacity", Alpha);
}

void UAttackIndicatorComponent::FinishFlash()
{
	MID_FlashIndicator->SetVectorParameterValue("Color", DefaultColor);
	MID_FlashIndicator->SetScalarParameterValue("Opacity", 1.0f);
}
