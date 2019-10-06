// Copyright Overthrone Studios 2019

#include "SwordComponent.h"
#include "Log.h"

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
}

void USwordComponent::Glow()
{
	if (GlowMaterial)
		SetMaterial(MaterialIndex, GlowMaterial);
	else
		ULog::Error(GetName() + ": Glow material does not exist", true);
}

void USwordComponent::Revert()
{
	SetMaterial(MaterialIndex, DefaultSwordMaterial);
}
