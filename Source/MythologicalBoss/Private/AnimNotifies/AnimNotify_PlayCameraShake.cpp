// Copyright Overthrone Studios 2019

#include "AnimNotifies/AnimNotify_PlayCameraShake.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequenceBase.h"
#include "Log.h"

void UAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// Don't call super to avoid call back in to blueprints

	const auto PlayerController = UGameplayStatics::GetPlayerController(MeshComp->GetOwner(), 0);
	const auto Player = UGameplayStatics::GetPlayerCharacter(MeshComp->GetOwner(), 0);

	if (PlayerController)
	{
		if (CameraShake)
		{
			const float Distance = FVector::Dist(Player->GetActorLocation(), MeshComp->GetOwner()->GetActorLocation());
			const float NewIntensity = FMath::GetMappedRangeValueClamped(FVector2D(1500.0f, 0.0f), FVector2D(0.0f, Intensity), Distance);

			PlayerController->ClientPlayCameraShake(CameraShake, NewIntensity);
		}
		else
			ULog::DebugMessage(WARNING, Animation->GetName() + ": Camera shake not found.", true);
	}
}
