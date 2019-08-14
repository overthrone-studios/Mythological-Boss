// Copyright Overthrone Studios 2019

#pragma once

#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBase.h"
#include "AnimNotifyState_ApplyDamagePlayer.generated.h"

/**
 * Applies damage during a period of an animation
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAnimNotifyState_ApplyDamagePlayer final : public UAnimNotifyState_ApplyDamageBase
{
	GENERATED_BODY()
	
protected:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void OnHit(USkeletalMeshComponent* MeshComp) override;

private:
	class AYlva* Ylva;
};
