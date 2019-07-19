// Copyright Overthrone Studios 2019

#include "Player/AnimNotifyState/ApplyDamage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UApplyDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	const FVector RightForeArm = MeshComp->GetSocketLocation(FName("RightForeArm"));
	const FVector RightHandLocation = MeshComp->GetSocketLocation(FName("RightHand"));
	FVector Direction = RightHandLocation - RightForeArm;
	Direction.Normalize();

	const FVector StartTrace = MeshComp->GetSocketLocation(FName("RightHand"));
	const FVector EndTrace = MeshComp->GetSocketLocation(FName("RightHand")) + Direction * 100.0f;

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(MeshComp, StartTrace, EndTrace, 10.0f, ETraceTypeQuery::TraceTypeQuery1, true, {}, EDrawDebugTrace::ForDuration, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		const auto HitActor = HitResult.GetActor();
		const FDamageEvent DamageEvent;

		if (HitActor)
			HitActor->TakeDamage(10.0f, DamageEvent, nullptr, nullptr);
	}
}
