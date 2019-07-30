// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Boss_ApplyDamage.generated.h"

/**
 * Applies damage during a period of an animation
 */
UCLASS()
class MYTHOLOGICALBOSS_API UBoss_ApplyDamage final : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
};
