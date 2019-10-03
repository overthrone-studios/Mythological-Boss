// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBase.h"
#include "HitSoundData.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_ApplyDamageBase::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	const FVector& StartTrace = MeshComp->GetSocketLocation(StartBone);
	const FVector& EndTrace = MeshComp->GetSocketLocation(EndBone);

	UKismetSystemLibrary::SphereTraceSingle(MeshComp, StartTrace, EndTrace, AttackRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, {}, DebugTrace, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if (HitResult.bBlockingHit && !bIsHit)
	{
		OnHit(MeshComp);
	}
}

void UAnimNotifyState_ApplyDamageBase::OnHit(USkeletalMeshComponent* MeshComp)
{
	check(0 && "You must implement OnHit()");
}

void UAnimNotifyState_ApplyDamageBase::PlayHitSound(UObject* WorldContextObject)
{
	if (HitSoundData && HitSoundData->HitSounds.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, HitSoundData->HitSounds.Num() - 1);

		float NewPitch = 1.0f;

		if (RandomDeviation > 0.0f)
		{
			const float MinPitch = Pitch - RandomDeviation;
			const float MaxPitch = Pitch + RandomDeviation;
			NewPitch = FMath::FRandRange(MinPitch, MaxPitch);
		}

		if (HitSoundData->HitSounds.IsValidIndex(Index))
			UGameplayStatics::PlaySoundAtLocation(WorldContextObject, HitSoundData->HitSounds[Index], HitResult.Location, FRotator(0.0f), 1.0f, NewPitch);
	}
}
