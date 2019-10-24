// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBase.h"
#include "AnimNotifyState_MordathKick.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAnimNotifyState_MordathKick : public UAnimNotifyState_ApplyDamageBase
{
	GENERATED_BODY()
	
protected:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void OnHit(USkeletalMeshComponent* MeshComp) override;
	
};
