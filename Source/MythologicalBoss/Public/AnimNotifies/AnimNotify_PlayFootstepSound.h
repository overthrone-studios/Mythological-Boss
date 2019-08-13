// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/FootstepCollection.h"
#include "AnimNotify_PlayFootstepSound.generated.h"

/**
 * Plays a footstep sound based on the material the character is moving on
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAnimNotify_PlayFootstepSound final : public UAnimNotify
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditInstanceOnly, Category = "Footstep", meta = (ToolTip = "Enable/Disable the ability to play footsteps?"))
		bool bEnableFootsteps = true;

	UPROPERTY(EditInstanceOnly, Category = "Footstep", meta = (EditCondition = "bEnableFootsteps", ToolTip = "An array of footstep data assets to play depending on the material the character is moving on"))
		UFootstepCollection* FootstepCollection;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// Plays a footstep sound based on the material the character is moving on
	void PlayFootstepSound();

	// Returns the footstep sound asset based on the physical material it's moving on
	USoundBase* GetFootstepSound(TWeakObjectPtr<UPhysicalMaterial>* Surface);

private:
	FFindFloorResult FloorResult;

	ACharacter* Character;
};

