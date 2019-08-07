// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayCameraShake.generated.h"

/**
 * Plays a camera shake to the main camera
 */
UCLASS()
class MYTHOLOGICALBOSS_API UAnimNotify_PlayCameraShake final : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditInstanceOnly, Category = "Camera Shake")
		TSubclassOf<class UCameraShake> CameraShake;

	UPROPERTY(EditInstanceOnly, Category = "Camera Shake", meta = (ClampMin = 0.0f, ClampMax = 1000.0f))
		float Intensity = 1.0f;
	
};
