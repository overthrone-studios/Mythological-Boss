// Copyright Overthrone Studios 2019

#pragma once

#include "AnimNotifyState_ApplyDamageBase.h"
#include "AnimNotifyState_ApplyDamageBoss.generated.h"

/**
 * Applies damage during a period of an animation
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAnimNotifyState_ApplyDamageBoss final : public UAnimNotifyState_ApplyDamageBase
{
	GENERATED_BODY()
	
protected:
	UAnimNotifyState_ApplyDamageBoss();

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void OnHit(USkeletalMeshComponent* MeshComp) override;

private:
	class AMordath* Mordath;
};
