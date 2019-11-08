// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamagePlayer.h"
#include "DestructibleActor.h"
#include "Components/HitboxComponent.h"
#include "Ylva.h"
#include "Log.h"
#include "HitSoundData.h"
#include "Animation/AnimSequenceBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void UAnimNotifyState_ApplyDamagePlayer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));

	Ylva = Cast<AYlva>(MeshComp->GetOwner());

	if (!Ylva)
		return;

#if !UE_BUILD_SHIPPING
	if (Ylva->Debug.bShowRaycasts)
		DebugTrace = EDrawDebugTrace::ForDuration;
	else
		DebugTrace = EDrawDebugTrace::None;
#endif

	//AttackRadius = Ylva->GetAttackRadius();

	if (Ylva->IsLightAttacking())
		AttackDamage = Ylva->GetLightAttackDamage();
	else if (Ylva->IsHeavyAttacking())
		AttackDamage = Ylva->GetHeavyAttackDamage();
	else if (Ylva->IsChargeAttacking())
		AttackDamage = Ylva->GetChargeAttackDamage();
	else if (Ylva->IsDashAttacking())
		AttackDamage = Ylva->GetDashAttackDamage();

#if !UE_BUILD_SHIPPING
	if (HitSoundData && HitSoundData->HitSounds.Num() == 0)
		ULog::Warning("No hit sound specified in " + Animation->GetName(), true);
#endif
}

void UAnimNotifyState_ApplyDamagePlayer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	bIsHit = false;
}

void UAnimNotifyState_ApplyDamagePlayer::OnHit(USkeletalMeshComponent* MeshComp, const FHitResult& HitResult)
{
	const auto HitActor = HitResult.GetActor();
	const auto HitComp = HitResult.GetComponent();
	const FDamageEvent DamageEvent;

	float Multiplier = 1.0f;

	if (Ylva && HitActor && HitActor->bCanBeDamaged && !Ylva->IsHealthZero())
	{
		bIsHit = true;

		Ylva->ApplyHitStop();

		Ylva->VibrateController(Ylva->GetCurrentForceFeedback());

		if (HitActor->IsA(ADestructibleActor::StaticClass()))
		{
			HitActor->TakeDamage(AttackDamage * Multiplier, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());
			return;
		}

		if (HitComp->IsA(UHitboxComponent::StaticClass()))
		{
			Multiplier = Cast<UHitboxComponent>(HitComp)->GetScalarValue();
			
			#if !UE_BUILD_SHIPPING
			if (Ylva->Debug.bLogComponentHits)
				DrawDebugString(MeshComp->GetOwner()->GetWorld(), HitResult.Location, HitComp->GetName(), nullptr, FColor::White, 1.0f);
			#endif
		}

		HitActor->TakeDamage(AttackDamage * Multiplier, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

		// Play sound effect
		PlayHitSound(MeshComp, HitResult);

		Ylva->OnAttackLanded(HitResult);
	}
}
