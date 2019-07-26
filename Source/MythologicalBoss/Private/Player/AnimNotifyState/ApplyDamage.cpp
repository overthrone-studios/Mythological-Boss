// Copyright Overthrone Studios 2019

#include "Player/AnimNotifyState/ApplyDamage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Ylva.h"

void UApplyDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	const FVector RightForeArm = MeshComp->GetSocketLocation(FName("RightForeArm"));
	const FVector RightHandLocation = MeshComp->GetSocketLocation(FName("RightHand"));
	FVector Direction = RightHandLocation - RightForeArm;
	Direction.Normalize();

	const auto Ylva = Cast<AYlva>(MeshComp->GetOwner());
	float AttackDistance = 100.0f;
	float AttackRadius = 10.0f;
	if (Ylva)
	{
		AttackDistance = Ylva->GetAttackRange();
		AttackRadius = Ylva->GetAttackRadius();
	}

	const FVector StartTrace = MeshComp->GetSocketLocation(FName("RightHand"));
	const FVector EndTrace = MeshComp->GetSocketLocation(FName("RightHand")) + Direction * AttackDistance;

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(MeshComp, StartTrace, EndTrace, AttackRadius, ETraceTypeQuery::TraceTypeQuery1, true, {}, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if (HitResult.bBlockingHit)
	{
		const auto HitActor = HitResult.GetActor();
		const FDamageEvent DamageEvent;
		
		float AttackDamage = 10.0f;

		if (Ylva)
			AttackDamage = Ylva->GetLightAttackDamage();

		if (HitActor)
			HitActor->TakeDamage(AttackDamage, DamageEvent, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());
	}
}
