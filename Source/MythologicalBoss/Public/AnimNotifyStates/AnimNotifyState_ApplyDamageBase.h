// Copyright Overthrone Studios 2019

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AnimNotifyState_ApplyDamageBase.generated.h"

/**
 * Base class for an apply damage notify
 */
UCLASS(Abstract)
class MYTHOLOGICALBOSS_API UAnimNotifyState_ApplyDamageBase : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void OnHit(USkeletalMeshComponent* MeshComp, const FHitResult& HitResult);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		FName StartBone = "RightHand";

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		FName EndBone = "SwordEnd";

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		class UHitSoundData* HitSoundData;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float Pitch = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
		float RandomDeviation = 0.2f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
		TSubclassOf<UDamageType> DamageType;

	void PlayHitSound(UObject* WorldContextObject, const FHitResult& HitResult);

	TArray<FHitResult> HitResults;

	float AttackDamage = 100.0f;
	float AttackRadius = 10.0f;

	uint8 bIsHit : 1;

	enum EDrawDebugTrace::Type DebugTrace;

	class AOverthroneCharacter* OverthroneCharacter;
};
