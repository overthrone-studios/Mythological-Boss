// Copyright Overthrone Studios 2019

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ApplyDamage.generated.h"

/**
 * Applies damage during a period of an animation
 */
UCLASS()
class MYTHOLOGICALBOSS_API UApplyDamage final : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};
