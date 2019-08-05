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
	SpecialAttack_3
};

/**
 *	An asset containing attack types
 */
UCLASS()
class MYTHOLOGICALBOSS_API UComboData final : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		void Init();

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		void NextAttack();

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		bool IsAtLastAttack();

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		int32 GetCurrentAttack() const { return AttackIndex; }

	UPROPERTY(EditInstanceOnly, Category = "Combos")
		TArray<TEnumAsByte<EAttackType_Combo>> Attacks;

	TEnumAsByte<EAttackType_Combo> CurrentAttack = LightAttack_1;

private:
	int32 AttackIndex = 0;
};
