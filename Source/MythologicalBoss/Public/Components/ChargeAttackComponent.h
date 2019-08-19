// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChargeAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Tags", "Activation", "Cooking", "AssetUserData", "Collision"))
class MYTHOLOGICALBOSS_API UChargeAttackComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChargeAttackComponent();

protected:
	void BeginPlay() override;

};
