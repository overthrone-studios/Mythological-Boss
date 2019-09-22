// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SubclassOf.h"
#include "OverthroneStructs.generated.h"

USTRUCT(BlueprintType)
struct FCameraShakeData
{
	GENERATED_BODY()

	// The camera shake to play
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TSubclassOf<class UCameraShake> Shake;

	// The intensity of the camera shake
	UPROPERTY(EditInstanceOnly, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float Intensity = 1.0f;
};

USTRUCT(BlueprintType)
struct FAttackSettings
{
	GENERATED_BODY()

	float OriginalLightAttackDamage = 50.0f;
	float OriginalHeavyAttackDamage = 100.0f;

	// The attack damage we deal when light attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float LightAttackDamage = 50.0f;

	// The attack damage we deal when heavy attacking
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 10000.0f))
		float HeavyAttackDamage = 100.0f;

	// The radius of the sphere raycast when attacking light or heavy
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float AttackRadius = 20.0f;

	// The radius of the sphere raycast when attacking light or heavy on low health
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float AttackRadiusOnLowHealth = 40.0f;
};

USTRUCT(BlueprintType)
struct FCombatSettings
{
	GENERATED_BODY()

	// Should we stop animations when we are hit?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		uint8 bEnableHitStop : 1;

	// The amount of time (in seconds) we stay paused when we hit something
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.001f, ClampMax = 10.0f, EditCondition = "bEnableHitStop"))
		float HitStopTime = 0.1f;
};

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UOverthroneStructs final : public UObject
{
	GENERATED_BODY()

public:
	UOverthroneStructs();
	~UOverthroneStructs();
};
