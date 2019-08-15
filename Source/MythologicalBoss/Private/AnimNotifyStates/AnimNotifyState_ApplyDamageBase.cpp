// Copyright Overthrone Studios 2019

#include "AnimNotifyStates/AnimNotifyState_ApplyDamageBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_ApplyDamageBase::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	const FVector RightForeArm = MeshComp->GetSocketLocation(FName("RightForeArm"));
	const FVector RightHandLocation = MeshComp->GetSocketLocation(StartBone);
	FVector Direction = RightHandLocation - RightForeArm;
	Direction.Normalize();

	const FVector StartTrace = MeshComp->GetSocketLocation(StartBone);
	const FVector EndTrace = MeshComp->GetSocketLocation(EndBone);

	UKismetSystemLibrary::SphereTraceSingle(MeshComp, StartTrace, EndTrace, AttackRadius, ETraceTypeQuery::TraceTypeQuery1, true, {}, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.0f);

	if (HitResult.bBlockingHit && !bIsHit)
	{
		OnHit(MeshComp);
	}
}

void UAnimNotifyState_ApplyDamageBase::OnHit(USkeletalMeshComponent* MeshComp)
{
	check(0 && "You must implement OnHit()");
}