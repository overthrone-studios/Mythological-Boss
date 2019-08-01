// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboData.generated.h"

UENUM()
enum EAttackType_Combo
{
	LightAttack_1,
	LightAttack_2,
	LightAttack_3,
	HeavyAttack_1,
	HeavyAttack_2,
	HeavyAttack_3,
	SpecialAttack_1,
	SpecialAttack_2,
	SpecialAttack_3,
};

/**
 *	An asset containing attack types
 */
UCLASS()
class MYTHOLOGICALBOSS_API UComboData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, Category = "Attacks")
		TEnumAsByte<EAttackType_Combo> FirstAttack;
	UPROPERTY(EditInstanceOnly, Category = "Attacks")
		TEnumAsByte<EAttackType_Combo> SecondAttack;
	UPROPERTY(EditInstanceOnly, Category = "Attacks")
		TEnumAsByte<EAttackType_Combo> ThirdAttack;
};
