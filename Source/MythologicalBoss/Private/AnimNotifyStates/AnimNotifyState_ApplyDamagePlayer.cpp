// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamagePlayer.h"
#include "DestructibleActor.h"
#include "Components/HitboxComponent.h"
#include "Ylva.h"
#include "Log.h"
#include "Animation/AnimSequenceBase.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_ApplyDamagePlayer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Ylva = Cast<AYlva>(MeshComp->GetOwner());

	if (!Ylva)
		return;

	if (Ylva->Debug.bShowRaycasts)
		DebugTrace = EDrawDebugTrace::ForDuration;
	else
		DebugTrace = EDrawDebugTrace::None;

	AttackRadius = Ylva->GetAttackRadius();

	if (Ylva->IsLightAttacking())
		AttackDamage = Ylva->GetLightAttackDamage();
	else if (Ylva->IsHeavyAttacking())
		AttackDamage = Ylva->GetHeavyAttackDamage();
	else if (Ylva->IsChargeAttacking())
		AttackDamage = Ylva->GetChargeAttackDamage();

	if (HitSounds.Num() == 0)
		ULog::Warning("No hit sound specified in " + Animation->GetName() + " is empty!", true);
}

void UAnimNotifyState_ApplyDamagePlayer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	bIsHit = false;
}

void UAnimNotifyState_ApplyDamagePlayer::OnHit(USkeletalMeshComponent* MeshComp)
{
	const auto HitActor = HitResult.GetActor();
	const auto HitComp = HitResult.GetComponent();
	const FDamageEvent DamageEvent;

	float Multiplier = 1.0f;

	if (HitActor && (HitActor->IsA(ACharacter::StaticClass()) || HitActor->IsA(ADestructibleActor::StaticClass())))
	{
		bIsHit = true;

		Ylva->ApplyHitStop();

		// Give charge
		Ylva->IncreaseCharge();

		if (HitComp->IsA(UHitboxComponent::StaticClass()))
		{
			Multiplier = Cast<UHitboxComponent>(HitComp)->GetScalarValue();
			
			if (Ylva->Debug.bLogComponentHits)
				ULog::Info(HitComp->GetName(), true);
		}

		HitActor->TakeDamage(AttackDamage * Multiplier, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

		// Play sound effect
		PlayHitSound(MeshComp);
	}
}
