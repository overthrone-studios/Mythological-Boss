// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBoss.h"
#include "ApexDestruction/Public/DestructibleActor.h"
#include "Animation/AnimSequenceBase.h"
#include "HitSoundData.h"
#include "Mordath.h"
#include "Log.h"
#include "Kismet/GameplayStatics.h"
#include "MordathGhost.h"
#include "DmgType_MordathKick.h"

UAnimNotifyState_ApplyDamageBoss::UAnimNotifyState_ApplyDamageBoss()
{
	StartBone = "SpearStart";
	EndBone = "SpearEnd";
}

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
	AttackDamage = DamageType == UDmgType_MordathKick::StaticClass() ? 0.0f : Mordath->GetActionDamage();

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
	const FDamageEvent DamageEvent = FDamageEvent(DamageType);

	if (HitComponent && Mordath)
		HitComponent->OnComponentHit.Broadcast(HitResult.GetComponent(), Mordath, MeshComp, HitResult.ImpactNormal, HitResult);

	if (HitActor && Mordath)
		UGameplayStatics::SpawnForceFeedbackAtLocation(MeshComp, Mordath->GetCurrentForceFeedbackEffect(), HitResult.Location);

	if (HitActor && (HitActor->IsA(ACharacter::StaticClass()) && HitActor->bCanBeDamaged) && !Mordath->IsHealthZero())
	{
		if (Mordath && Mordath->IsDamaged())
			return;

		bIsHit = true;

		// Apply hit stop
		if (Mordath)
			Mordath->PauseAnimsWithTimer();

		HitActor->TakeDamage(AttackDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

		if (Mordath && !Mordath->IsDamaged() && !Mordath->IsStunned())
		{
			// Play sound effect
			PlayHitSound(MeshComp);

			Mordath->OnAttackLanded(HitResult);
		}
	}
}
