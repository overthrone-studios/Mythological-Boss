// Copyright Overthrone Studios 2019

#include "MordathSKMComponentBase.h"

#include "GameFramework/Character.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Curves/CurveFloat.h"

#include "OverthroneFunctionLibrary.h"

UMordathSKMComponentBase::UMordathSKMComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMordathSKMComponentBase::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	OriginalMaterial = GetMaterial(0);
	MID_OriginalMaterial = CreateDynamicMaterialInstance(0, OriginalMaterial, *("MID_"+GetName()));

	const FMaterialParameterInfo ParameterInfo{"Attack Color"};
	MID_OriginalMaterial->GetVectorParameterValue(ParameterInfo, OriginalActionColor);

	UOverthroneFunctionLibrary::SetupTimeline(this, TL_Dissolve, DissolveCurve, false, DissolveSpeed, "UpdateDissolve", "FinishDissolve");
}

void UMordathSKMComponentBase::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TL_Dissolve.TickTimeline(DeltaTime);
}

void UMordathSKMComponentBase::Reappear()
{
	MID_OriginalMaterial->SetScalarParameterValue("IsDissolving", 1.0f);

	OnBeginReappear.Broadcast();

	bWasReversing = true;
	TL_Dissolve.ReverseFromEnd();
}

void UMordathSKMComponentBase::Disappear()
{
	if (!TL_Dissolve.IsPlaying())
	{
		MID_OriginalMaterial->SetScalarParameterValue("IsDissolving", 1.0f);

		TL_Dissolve.Stop();
		TL_Dissolve.SetPlaybackPosition(0.0f, true, true);

		OnBeginDisappear.Broadcast();

		TL_Dissolve.PlayFromStart();
	}
}

void UMordathSKMComponentBase::UpdateVectorParameter(const FName& ParameterName, const FLinearColor& Vector)
{
	MID_OriginalMaterial->SetVectorParameterValue(ParameterName, Vector);
}

void UMordathSKMComponentBase::UpdateScalarParameter(const FName& ParameterName, const float Scalar)
{
	MID_OriginalMaterial->SetScalarParameterValue(ParameterName, Scalar);
}

void UMordathSKMComponentBase::ResetActionColor()
{
	MID_OriginalMaterial->SetVectorParameterValue("Attack Color", OriginalActionColor);
}

void UMordathSKMComponentBase::UpdateDissolve()
{
	const float Time = TL_Dissolve.GetPlaybackPosition();
	const float DissolveAmount = DissolveCurve->GetFloatValue(Time);

	MID_OriginalMaterial->SetScalarParameterValue("Amount", DissolveAmount);
}

void UMordathSKMComponentBase::FinishDissolve()
{
	if (bWasReversing)
	{
		bWasReversing = false;

		MID_OriginalMaterial->SetScalarParameterValue("IsDissolving", 0.0f);

		SetMaterial(0, MID_OriginalMaterial);

		OnReappeared.Broadcast();
	}
	else
	{
		OnDisappeared.Broadcast();
	}
}
