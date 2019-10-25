// Copyright Overthrone Studios 2019

#include "ANS_ApplyDamageMordathGhost.h"

#include "Boss/MordathGhost.h"

#include "Components/SkeletalMeshComponent.h"

#include "Animation/AnimSequenceBase.h"

#include "Misc/HitSoundData.h"

#include "Log.h"
#include "Kismet/GameplayStatics.h"


UANS_ApplyDamageMordathGhost::UANS_ApplyDamageMordathGhost()
{
	StartBone = "SpearStart";
	EndBone = "SpearEnd";
}

void UANS_ApplyDamageMordathGhost::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	MordathGhost = Cast<AMordathGhost>(MeshComp->GetOwner());

	if (!MordathGhost)
		return;

	AttackRadius = MordathGhost->GetAttackRadius();
	AttackDamage = MordathGhost->GetActionDamage();

#if !UE_BUILD_SHIPPING
	if (HitSoundData && HitSoundData->HitSounds.Num() == 0)
		ULog::Warning("No hit sounds specified in " + Animation->GetName(), true);
#endif
}

void UANS_ApplyDamageMordathGhost::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	bIsHit = false;
}

void UANS_ApplyDamageMordathGhost::OnHit(USkeletalMeshComponent* MeshComp)
{
	const auto HitActor = HitResult.GetActor();
	const auto HitComponent = HitResult.GetComponent();
	const FDamageEvent DamageEvent = FDamageEvent(DamageType);

	if (HitComponent && MordathGhost)
		HitComponent->OnComponentHit.Broadcast(HitResult.GetComponent(), MordathGhost, MeshComp, HitResult.ImpactNormal, HitResult);

	if (HitActor && MordathGhost)
		UGameplayStatics::SpawnForceFeedbackAtLocation(MeshComp, MordathGhost->GetCurrentForceFeedbackEffect(), HitResult.Location);

	if (HitActor && (HitActor->IsA(ACharacter::StaticClass()) && HitActor->bCanBeDamaged))
	{
		bIsHit = true;

		// Apply hit stop
		if (MordathGhost)
			MordathGhost->PauseAnimsWithTimer();

		HitActor->TakeDamage(AttackDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

		// Play sound effect
		PlayHitSound(MeshComp);
	}
}
