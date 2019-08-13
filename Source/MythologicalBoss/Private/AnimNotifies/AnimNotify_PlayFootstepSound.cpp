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
		if (IsValid(GetFootstepSound(&FloorResult.HitResult.PhysMaterial)))
		{
			UGameplayStatics::PlaySoundAtLocation(Character, GetFootstepSound(&FloorResult.HitResult.PhysMaterial), FloorResult.HitResult.Location);
		}
		else
		{
			ULog::DebugMessage(WARNING, "No physical material found for " + FloorResult.HitResult.GetActor()->GetName(), true);
		}
	}
}

USoundBase* UAnimNotify_PlayFootstepSound::GetFootstepSound(TWeakObjectPtr<UPhysicalMaterial>* Surface)
{
	for (auto FootstepData : FootstepCollection->GetFootstepMappings())
	{
		if (FootstepData->GetPhysicalMaterial() == Surface->Get())
			return FootstepData->GetFootstepSounds()[FMath::RandRange(0, FootstepData->GetFootstepSounds().Num() - 1)];
	}

	ULog::DebugMessage(WARNING, "No footstep sound", true);

	return nullptr;
}


