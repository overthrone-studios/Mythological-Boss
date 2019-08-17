// Copyright Overthrone Studios 2019

#include "AnimNotify_PlayFootstepSound.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Misc/FootstepData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Log.h"

void UAnimNotify_PlayFootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Character = UGameplayStatics::GetPlayerCharacter(MeshComp->GetOwner(), 0);
	
	if (bEnableFootsteps && Character)
		PlayFootstepSound();
}

void UAnimNotify_PlayFootstepSound::PlayFootstepSound()
{
	Character->GetCharacterMovement()->FindFloor(Character->GetCapsuleComponent()->GetComponentLocation(), FloorResult, false);

	if (FloorResult.bBlockingHit)
	{
		USoundBase* FootstepSound = GetFootstepSound(&FloorResult.HitResult.PhysMaterial);

		if (IsValid(FootstepSound))
		{
			UGameplayStatics::PlaySoundAtLocation(Character, FootstepSound, FloorResult.HitResult.Location);
		}
		else
		{
			if (FootstepCollection->bDebug)
				ULog::DebugMessage(WARNING, "No physical material found for " + FloorResult.HitResult.GetActor()->GetName(), true);
		}
	}
}

USoundBase* UAnimNotify_PlayFootstepSound::GetFootstepSound(TWeakObjectPtr<UPhysicalMaterial>* Surface) const
{
	for (auto FootstepData : FootstepCollection->GetFootstepMappings())
	{
		if (FootstepData->GetPhysicalMaterial() == Surface->Get())
		{
			USoundBase* ChosenSound = FootstepData->GetFootstepSounds()[FMath::RandRange(0, FootstepData->GetFootstepSounds().Num() - 1)];

			if (FootstepCollection->bDebug)
				ULog::DebugMessage(INFO, ChosenSound->GetName() + " selected", true);

			return ChosenSound;
		}
	}

	if (FootstepCollection->bDebug)
		ULog::DebugMessage(WARNING, FString("No footstep sound"), true);

	return nullptr;
}


