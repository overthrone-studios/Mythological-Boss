// Copyright Overthrone Studios 2019

#include "HitboxComponent.h"

UHitboxComponent::UHitboxComponent()
{
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
}
