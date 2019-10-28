// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBase.h"
#include "ANS_ApplyDamageMordathGhost.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UANS_ApplyDamageMordathGhost : public UAnimNotifyState_ApplyDamageBase
{
	GENERATED_BODY()

protected:
	UANS_ApplyDamageMordathGhost();

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void OnHit(USkeletalMeshComponent* MeshComp, const FHitResult& HitResult) override;

private:
	class AMordathGhost* MordathGhost;
};
