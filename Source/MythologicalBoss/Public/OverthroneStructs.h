// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "Log.h"
#include <chrono>
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

USTRUCT(BlueprintType)
struct FThinkStateData
{
	GENERATED_BODY()

		// Calculates a new think time using RandomDeviation
		float CalculateThinkTime();

protected:
	// How long (in seconds) should the boss think for, before initiating an attack?
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 60.0f))
		float ThinkTime = 2.0f;

	// Adds a random range to ThinkTime
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f))
		float RandomDeviation = 0.3f;
};

USTRUCT(BlueprintType)
struct FRetreatStateData
{
	GENERATED_BODY()

		// Calculates a new retreat time using RandomDeviation
		float CalculateRetreatTime();

protected:
	// How long (in seconds) should the boss move away from you (walk backwards)?
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f, ClampMax = 60.0f))
		float RetreatTime = 1.0f;

	// Adds a random range to RetreatTime
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ClampMin = 0.0f))
		float RandomDeviation = 0.3f;
};

typedef std::ratio<1, 1000000000000> pico;
typedef std::chrono::duration<long long, pico> picoseconds;

typedef std::ratio<1, 1000000000000000> femto;
typedef std::chrono::duration<long long, femto> femtoseconds;

typedef std::ratio<1, 1000000000000000000> atto;
typedef std::chrono::duration<long long, atto> attoseconds;

struct MyTimer
{
	MyTimer()
	{
		StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	~MyTimer()
	{
		Stop();
	}

	void Stop()
	{
		const auto& EndTimePoint = std::chrono::high_resolution_clock::now();

		const long long& Duration = std::chrono::duration_cast<picoseconds>(EndTimePoint - StartTimePoint).count();

		const double& US = Duration * 0.000001;

		ULog::Info("Duration " + FString::SanitizeFloat(US) + "us", true);
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> StartTimePoint;
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
