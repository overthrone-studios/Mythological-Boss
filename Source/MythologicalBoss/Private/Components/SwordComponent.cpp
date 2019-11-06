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

	MID_SwordMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DefaultSwordMaterial, FName("MID_Sword"));
	MID_SwordMaterial->SetScalarParameterValue(EmissiveParameterName, 0.0f);

	SetMaterial(MaterialIndex, MID_SwordMaterial);
}

void USwordComponent::Glow()
{
	MID_SwordMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, DefaultSwordMaterial, FName("MID_Sword"));
	MID_SwordMaterial->SetScalarParameterValue(EmissiveParameterName, EmissiveStrength);

	SetMaterial(MaterialIndex, MID_SwordMaterial);

}

void USwordComponent::Revert()
{
	SetMaterial(MaterialIndex, DefaultSwordMaterial);
}

UMaterialInterface* USwordComponent::GetDefaultSwordMaterial() const
{
	return DefaultSwordMaterial;
}
