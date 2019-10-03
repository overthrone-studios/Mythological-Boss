// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayForceFeedback.generated.h"

/**
 * Plays a force feedback effect on the current controller
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAnimNotify_PlayForceFeedback final : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	// The force feedback effect to play
	UPROPERTY(EditInstanceOnly, Category = "Force Feedback")
		class UForceFeedbackEffect* ForceFeedback;

	// The bone location to play the force feedback effect at
	UPROPERTY(EditInstanceOnly, Category = "Force Feedback")
		FName Bone = "ROOT_jnt";
};
