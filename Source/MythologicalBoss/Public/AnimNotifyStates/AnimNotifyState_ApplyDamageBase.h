// Copyright Overthrone Studios 2019

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "K2Node_FunctionResult.h"
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

	virtual void OnHit(USkeletalMeshComponent* MeshComp);

	FHitResult HitResult;

	float AttackDamage = 100.0f;
	float AttackDistance = 100.0f;
	float AttackRadius = 10.0f;

	uint8 bIsHit : 1;

	class AOverthroneCharacter* OverthroneCharacter;
};
