// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBoss.h"
#include "ApexDestruction/Public/DestructibleActor.h"
#include "Animation/AnimSequenceBase.h"
#include "HitSoundData.h"
#include "Mordath.h"
#include "Log.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_ApplyDamageBoss::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Mordath = Cast<AMordath>(MeshComp->GetOwner());

	if (!Mordath)
		return;

#if !UE_BUILD_SHIPPING
	if (Mordath->Debug.bShowRaycasts)
		DebugTrace = EDrawDebugTrace::ForDuration;
	else
		DebugTrace = EDrawDebugTrace::None;
#endif

	AttackRadius = Mordath->GetAttackRadius();

	if (Mordath->IsShortAttacking())
		AttackDamage = Mordath->GetShortAttackDamage();
	else if (Mordath->IsLongAttacking())
		AttackDamage = Mordath->GetLongAttackDamage();
	else if (Mordath->IsSpecialAttacking())
		AttackDamage = Mordath->GetSpecialAttackDamage();

#if !UE_BUILD_SHIPPING
	if (HitSoundData && HitSoundData->HitSounds.Num() == 0)
		ULog::Warning("No hit sounds specified in " + Animation->GetName(), true);
#endif
}

void UAnimNotifyState_ApplyDamageBoss::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	bIsHit = false;
}

void UAnimNotifyState_ApplyDamageBoss::OnHit(USkeletalMeshComponent* MeshComp)
{
	const auto HitActor = HitResult.GetActor();
	const auto HitComponent = HitResult.GetComponent();
	const FDamageEvent DamageEvent;

	if (HitComponent)
	{
		HitComponent->OnComponentHit.Broadcast(HitResult.GetComponent(), Mordath, MeshComp, HitResult.ImpactNormal, HitResult);
		ULog::Info("Hit Component: " + HitComponent->GetName(), true);
	}

	if (HitActor && (HitActor->IsA(ACharacter::StaticClass()) && HitActor->bCanBeDamaged))
	{
		bIsHit = true;

		// Apply hit stop
		Mordath->PauseAnimsWithTimer();

		HitActor->TakeDamage(AttackDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

		if (!Mordath->IsDamaged() && !Mordath->IsStunned())
		{
			// Play sound effect
			PlayHitSound(MeshComp);
		}
	}
}

void UAnimNotifyState_ApplyDamageBoss::OnOverlap(USkeletalMeshComponent* MeshComp)
{
	const auto OverlappedActor = HitResult.GetActor();
	const auto OverlappedComponent = HitResult.GetComponent();

	ULog::ObjectValidity(OverlappedComponent, true);

	if (OverlappedActor)
		ULog::Info("Overlapped with " + OverlappedActor->GetName(), true);

	if (OverlappedComponent)
		ULog::Info("Overlapped with " + OverlappedComponent->GetName(), true);
}
