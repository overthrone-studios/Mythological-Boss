// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBase.h"
#include "AnimNotifyState_ApplyDamageAOE.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAnimNotifyState_ApplyDamageAOE : public UAnimNotifyState_ApplyDamageBase
{
	GENERATED_BODY()
	
protected:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void OnHit(USkeletalMeshComponent* MeshComp, const FHitResult& HitResult) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DisplayName = "AOE Radius")
		float AOERadius = 50.0f;

private:
	class AMordath* Mordath;
	class AMordathGhost* MordathGhost;
};
