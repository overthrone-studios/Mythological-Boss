// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboData.generated.h"

UENUM()
enum EDashType_Combo
{
	Dash_Forward,
	Dash_Backward,
	Dash_Left,
	Dash_Right,
};

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

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly)
		TEnumAsByte<EAttackType_Combo> Attack;

	UPROPERTY(EditInstanceOnly)
		uint8 bCanDashWithAttack : 1;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bCanDashWithAttack"))
		TEnumAsByte<EDashType_Combo> DashType;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition="bCanDashWithAttack"))
		float OffsetDistance = 0.0f;
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
		int32 GetCurrentAttackIndex() const { return AttackIndex; }

	UFUNCTION(BlueprintCallable, Category = "Combo Data")
		FAttackInfo GetCurrentAttackInfo() const { return CurrentAttack; }

protected:
	UPROPERTY(EditInstanceOnly, Category = "Combos")
		TArray<FAttackInfo> Attacks;

	FAttackInfo CurrentAttack;

private:
	int32 AttackIndex = 0;
};
