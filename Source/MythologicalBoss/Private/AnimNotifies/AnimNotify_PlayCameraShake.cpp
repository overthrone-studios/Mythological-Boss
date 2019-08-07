// Copyright Overthrone Studios 2019

#include "AnimNotifies/AnimNotify_PlayCameraShake.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"

void UAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// Don't call super to avoid call back in to blueprints

	const auto PlayerController = UGameplayStatics::GetPlayerController(MeshComp->GetOwner(), 0);

	if (PlayerController)
	{
		if (CameraShake)
			PlayerController->ClientPlayCameraShake(CameraShake, Intensity);
		else
			ULog::DebugMessage(WARNING, "Camera shake not found.", true);
	}
}
