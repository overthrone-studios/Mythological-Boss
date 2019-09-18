// Copyright Overthrone Studios 2019

#include "FlashIndicatorComponent.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Curves/CurveFloat.h"

#include "Engine/World.h"

#include "Log.h"

UFlashIndicatorComponent::UFlashIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetWorldScale3D(FVector(0.0f));
}

void UFlashIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize flash indicator
	if (FlashMaterial)
		SetMaterial(0, FlashMaterial);
	else
		ULog::Error(GetName() + ": There is no flash material!", true);

	MID_FlashIndicator = UMaterialInstanceDynamic::Create(FlashMaterial, this, "Flash Material Instance");
	SetWorldScale3D(FVector(0.0f));
	SetMaterial(0, MID_FlashIndicator);

	// Flash Timeline Initialization
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, "GrowFlash");
	TimelineFinishedCallback.BindUFunction(this, "FinishFlash");

	if (FlashCurve)
	{
		FlashTimeline.SetLooping(false);
		FlashTimeline.SetPlayRate(FlashSpeed);
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

void UFlashIndicatorComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FlashTimeline.TickTimeline(DeltaTime);
}

void UFlashIndicatorComponent::Flash(const FLinearColor& FlashColor)
{
	CurrentColor = FlashColor;
	MID_FlashIndicator->SetVectorParameterValue("Color", FlashColor);

	SetWorldScale3D(FVector(0.0f));

	FlashTimeline.PlayFromStart();
}

void UFlashIndicatorComponent::GrowFlash()
{
	const float Time = FlashTimeline.GetPlaybackPosition();
	const float Alpha = FlashCurve->GetFloatValue(FlashTimeline.GetPlaybackPosition());

	const FVector NewScale = FMath::Lerp(FVector(0.0f), FVector(1.0f), Alpha);
	
	if (Time > FlashCurve->FloatCurve.Keys[1].Time)
	{
		const float NewColorAlpha = FMath::GetMappedRangeValueClamped(FVector2D(1.0f, 0.0f), FVector2D(0.2f, 0.0f), Alpha);
		CurrentColor.A = NewColorAlpha;
		MID_FlashIndicator->SetVectorParameterValue("Color", CurrentColor);
	}
	else
		SetWorldScale3D(NewScale);
}

void UFlashIndicatorComponent::FinishFlash()
{
	SetWorldScale3D(FVector(0.0f));
}
