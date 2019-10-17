// Copyright Overthrone Studios 2019

#include "PlayerCameraComponent.h"

#include "OverthroneFunctionLibrary.h"

#include "Curves/CurveFloat.h"

UPlayerCameraComponent::UPlayerCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	UOverthroneFunctionLibrary::SetupTimeline(this, TL_ScreenSaturation, SaturationCurve, false, SaturationSpeed, "UpdateSaturation");
	UOverthroneFunctionLibrary::SetupTimeline(this, TL_Vignette, VignetteCurve, true, VignetteSpeed, "UpdateVignette");
}

void UPlayerCameraComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TL_ScreenSaturation.TickTimeline(DeltaTime);
	TL_Vignette.TickTimeline(DeltaTime);
}

void UPlayerCameraComponent::UpdateSaturation()
{
	const float Time = TL_ScreenSaturation.GetPlaybackPosition();
	const float CurveValue = SaturationCurve->GetFloatValue(Time);

	PostProcessSettings.bOverride_ColorSaturation = true;
	PostProcessSettings.ColorSaturation = FVector4(FVector(CurveValue), 1.0f);
}

void UPlayerCameraComponent::UpdateVignette()
{
	const float Time = TL_Vignette.GetPlaybackPosition();
	const float CurveValue = VignetteCurve->GetFloatValue(Time);

	PostProcessSettings.bOverride_VignetteIntensity = true;
	PostProcessSettings.VignetteIntensity = CurveValue;
}

void UPlayerCameraComponent::DesaturateScreen()
{
	TL_ScreenSaturation.PlayFromStart();
}

void UPlayerCameraComponent::ResaturateScreen()
{
	TL_ScreenSaturation.ReverseFromEnd();
}

void UPlayerCameraComponent::OscillateVignette()
{
	TL_Vignette.PlayFromStart();
}

void UPlayerCameraComponent::StopOscillatingVignette()
{
	TL_Vignette.Stop();
}
