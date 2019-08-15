// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBoss.h"
#include "ApexDestruction/Public/DestructibleActor.h"
#include "Mordath.h"

void UAnimNotifyState_ApplyDamageBoss::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Mordath = Cast<AMordath>(MeshComp->GetOwner());

	if (!Mordath)
		return;

	AttackDistance = Mordath->GetAttackRange();
	AttackRadius = Mordath->GetAttackRadius();

	if (Mordath->IsLightAttacking())
		AttackDamage = Mordath->GetLightAttackDamage();
	else if (Mordath->IsHeavyAttacking())
		AttackDamage = Mordath->GetHeavyAttackDamage();
	else if (Mordath->IsSpecialAttacking())
		AttackDamage = Mordath->GetSpecialAttackDamage();
}

void UAnimNotifyState_ApplyDamageBoss::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	bIsHit = false;
}

void UAnimNotifyState_ApplyDamageBoss::OnHit(USkeletalMeshComponent* MeshComp)
{
	const auto HitActor = HitResult.GetActor();
	const FDamageEvent DamageEvent;

	if (HitActor && (HitActor->IsA(ACharacter::StaticClass()) || HitActor->IsA(ADestructibleActor::StaticClass())))
	{
		bIsHit = true;

		// Apply hit stop
		Mordath->PauseAnimsWithTimer();

		HitActor->TakeDamage(AttackDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());
	}
}
