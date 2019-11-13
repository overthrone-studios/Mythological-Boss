// Copyright Overthrone Studios 2019

#include "PlayerCameraComponent.h"

#include "OverthroneFunctionLibrary.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"

#include "Curves/CurveFloat.h"

#include "Kismet/GameplayStatics.h"

UPlayerCameraComponent::UPlayerCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	Owner = GetOwner();

	UOverthroneFunctionLibrary::SetupTimeline(this, TL_ScreenSaturation, SaturationCurve, false, SaturationSpeed, "UpdateSaturation");
	UOverthroneFunctionLibrary::SetupTimeline(this, TL_Vignette, VignetteCurve, true, VignetteSpeed, "UpdateVignette");
}

void UPlayerCameraComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TL_ScreenSaturation.TickTimeline(DeltaTime);
	TL_Vignette.TickTimeline(DeltaTime);
}

#pragma region Lock-On
void UPlayerCameraComponent::ToggleLockOn()
{
	bWasLockedOn = bIsLockedOn;
	bIsLockedOn = !bIsLockedOn;

	bIsLockedOn ? EnableLockOn() : DisableLockOn();
}

void UPlayerCameraComponent::EnableLockOn()
{
	bIsLockedOn = true;
	PlayerController->SetIgnoreLookInput(true);

	OnEnableLockOn.Broadcast();
}

void UPlayerCameraComponent::DisableLockOn()
{
	bIsLockedOn = false;
	PlayerController->SetIgnoreLookInput(false);

	OnDisableLockOn.Broadcast();
}
#pragma endregion

FVector UPlayerCameraComponent::GetCurrentLockOnTargetLocation(TArray<ACharacter*> Targets, const FName SocketName) const
{
	if (SocketName == NAME_None)
	{
		if (Targets.IsValidIndex(CurrentLockOnIndex))
			return Targets[CurrentLockOnIndex]->GetActorLocation();
	}
	else
	{
		if (Targets.IsValidIndex(CurrentLockOnIndex))
			return Targets[CurrentLockOnIndex]->GetMesh()->GetSocketLocation(SocketName);
	}

	return FVector(0.0f);
}

FVector UPlayerCameraComponent::CycleLockOnTargets(TArray<class ACharacter*> Targets, const FName SocketName)
{
	if (CurrentLockOnIndex >= Targets.Num() - 1)
		CurrentLockOnIndex = 0;
	else
		++CurrentLockOnIndex;

	if (Targets.IsValidIndex(CurrentLockOnIndex))
		return Targets[CurrentLockOnIndex]->GetMesh()->GetSocketLocation(SocketName);

	return FVector(0.0f);
}

ACharacter* UPlayerCameraComponent::DetermineClosestLockOnTarget(TArray<ACharacter*> Targets)
{
	if (Targets.Num() == 0)
		return nullptr;

	float CurrentClosestDistance = TNumericLimits<float>::Max();

	uint8 i = 0;
	for (auto Mordath : Targets)
	{
		const float Distance = FVector::Dist(Owner->GetActorLocation(), Mordath->GetActorLocation());

		if (Distance < CurrentClosestDistance)
		{
			CurrentClosestDistance = Distance;
			ClosestTarget = Mordath;
			CurrentLockOnIndex = i;
		}

		++i;
	}

	return ClosestTarget;
}

void UPlayerCameraComponent::ResetLockOnTarget()
{
	CurrentLockOnIndex = 0;
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
	TL_Vignette.SetLooping(true);
	TL_Vignette.PlayFromStart();
}

void UPlayerCameraComponent::StopOscillatingVignette()
{
	TL_Vignette.SetLooping(false);
}
