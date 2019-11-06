// Copyright Overthrone Studios 2019

#include "AttackIndicatorComponent.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Curves/CurveFloat.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"


#include "Log.h"

UAttackIndicatorComponent::UAttackIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
}

void UAttackIndicatorComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FlashTimeline.TickTimeline(DeltaTime);
}

void UAttackIndicatorComponent::Flash(const FLinearColor& FlashColor)
{
	//MID_FlashIndicator->SetVectorParameterValue("Color", FlashColor);
	//MID_FlashIndicator->SetScalarParameterValue("Opacity", 0.0f);
	//
	//FlashTimeline.PlayFromStart();
}

void UAttackIndicatorComponent::ReassignMaterial()
{
	MID_FlashIndicator = Owner->GetMesh()->CreateDynamicMaterialInstance(0, FlashMaterial, "MID_AttackIndicator");
	MID_FlashIndicator->SetScalarParameterValue("Opacity", 1.0f);
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
