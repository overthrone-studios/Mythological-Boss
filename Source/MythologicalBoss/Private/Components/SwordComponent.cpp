// Copyright Overthrone Studios 2019

#include "SwordComponent.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Kismet/KismetMaterialLibrary.h"

USwordComponent::USwordComponent()
{
	UStaticMeshComponent::SetCollisionProfileName(FName("NoCollision"));
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USwordComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	DefaultSwordMaterial = GetMaterial(MaterialIndex);

	//MID_SwordMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DefaultSwordMaterial, FName("MID_Sword"));
	//MID_SwordMaterial->SetScalarParameterValue(EmissiveParameterName, 0.0f);

	MID_SwordMaterial = CreateDynamicMaterialInstance(0, DefaultSwordMaterial, FName("MID_Sword"));
	MID_SwordMaterial->SetScalarParameterValue(EmissiveParameterName, 0.0f);
}

void USwordComponent::Glow()
{
	MID_SwordMaterial->SetScalarParameterValue(EmissiveParameterName, EmissiveStrength);
}

void USwordComponent::Revert()
{
	MID_SwordMaterial->SetScalarParameterValue(EmissiveParameterName, 0.0f);
}

UMaterialInterface* USwordComponent::GetDefaultSwordMaterial() const
{
	return DefaultSwordMaterial;
}
