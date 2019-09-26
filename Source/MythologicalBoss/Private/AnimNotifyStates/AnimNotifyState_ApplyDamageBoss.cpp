// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBoss.h"
#include "ApexDestruction/Public/DestructibleActor.h"
#include "Animation/AnimSequenceBase.h"
#include "HitSoundData.h"
#include "Mordath.h"
#include "Log.h"
#include "Kismet/GameplayStatics.h"
#include "MordathGhost.h"

void UAnimNotifyState_ApplyDamageBoss::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Mordath = Cast<AMordath>(MeshComp->GetOwner());

	if (!Mordath)
	{
		MordathGhost = Cast<AMordathGhost>(MeshComp->GetOwner());

		AttackRadius = MordathGhost->GetAttackRadius();

		if (MordathGhost->IsShortAttacking())
			AttackDamage = MordathGhost->GetShortAttackDamage();
		else if (MordathGhost->IsLongAttacking())
			AttackDamage = MordathGhost->GetLongAttackDamage();
		else if (MordathGhost->IsSpecialAttacking())
			AttackDamage = MordathGhost->GetSpecialAttackDamage();

		return;
	}

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
		HitComponent->OnComponentHit.Broadcast(HitResult.GetComponent(), Mordath, MeshComp, HitResult.ImpactNormal, HitResult);

	if (HitActor && (HitActor->IsA(ACharacter::StaticClass()) && HitActor->bCanBeDamaged))
	{
		bIsHit = true;

		// Apply hit stop
		if (Mordath)
			Mordath->PauseAnimsWithTimer();
		else if (MordathGhost)
			MordathGhost->PauseAnimsWithTimer();

		HitActor->TakeDamage(AttackDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

		if (Mordath && !Mordath->IsDamaged() && !Mordath->IsStunned())
		{
			// Play sound effect
			PlayHitSound(MeshComp);
		}
		else if(MordathGhost)
		{
			// Play sound effect
			PlayHitSound(MeshComp);
		}
	}
}
