// Copyright Overthrone Studios 2019

#include "MordathFeatherComponent.h"

UMordathFeatherComponent::UMordathFeatherComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMordathFeatherComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalMaterial = GetMaterial(0);
	MID_OriginalMaterial = CreateDynamicMaterialInstance(0, OriginalMaterial, "MID_Mordath");
}

void UMordathFeatherComponent::RevertMaterial()
{
	MID_OriginalMaterial = CreateDynamicMaterialInstance(0, OriginalMaterial, "MID_Mordath");
}
