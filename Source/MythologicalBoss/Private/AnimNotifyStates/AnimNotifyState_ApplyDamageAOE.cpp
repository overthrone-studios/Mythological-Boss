// Copyright Overthrone Studios 2019

#include "AnimNotifyState_ApplyDamageAOE.h"

#include "Mordath.h"
#include "MordathGhost.h"

#include "Misc/HitSoundData.h"

#include "Animation/AnimSequenceBase.h"

#include "Log.h"
#include "Kismet/GameplayStatics.h"
#include "DmgType_AOE.h"

void UAnimNotifyState_ApplyDamageAOE::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Mordath = Cast<AMordath>(MeshComp->GetOwner());

	if (!Mordath)
	{
		MordathGhost = Cast<AMordathGhost>(MeshComp->GetOwner());

		if (!MordathGhost)
			return;

		AttackDamage = MordathGhost->GetActionDamage();

		return;
	}

#if !UE_BUILD_SHIPPING
	if (Mordath->Debug.bShowRaycasts)
		DebugTrace = EDrawDebugTrace::ForDuration;
	else
		DebugTrace = EDrawDebugTrace::None;
#endif

	AttackRadius = AOERadius;
	AttackDamage = Mordath->GetActionDamage();

#if !UE_BUILD_SHIPPING
	if (HitSoundData && HitSoundData->HitSounds.Num() == 0)
		ULog::Warning("No hit sounds specified in " + Animation->GetName(), true);
#endif
}

void UAnimNotifyState_ApplyDamageAOE::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	bIsHit = false;
}

void UAnimNotifyState_ApplyDamageAOE::OnHit(USkeletalMeshComponent* MeshComp)
{
	const auto HitActor = HitResult.GetActor();
	const FDamageEvent DamageEvent = FDamageEvent(UDmgType_AOE::StaticClass());

	if (HitActor && Mordath)
	{
		UGameplayStatics::SpawnForceFeedbackAtLocation(MeshComp, Mordath->GetCurrentForceFeedbackEffect(), HitResult.Location);
		ULog::Info(HitActor->GetName(), true);
	}

	if (HitActor && (HitActor->IsA(ACharacter::StaticClass()) && HitActor->bCanBeDamaged) && Mordath && !Mordath->IsHealthZero())
	{
		bIsHit = true;

		HitActor->TakeDamage(AttackDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

		if (Mordath && !Mordath->IsDamaged() && !Mordath->IsStunned())
		{
			// Play sound effect
			PlayHitSound(MeshComp);

			Mordath->OnAttackLanded(HitResult);
		}
		else if (MordathGhost)
		{
			// Play sound effect
			PlayHitSound(MeshComp);
		}
	}
}