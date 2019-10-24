// Copyright Overthrone Studios 2019

#include "AnimNotifyState_MordathKick.h"
#include "Mordath.h"


void UAnimNotifyState_MordathKick::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//Mordath = Cast<AMordath>(MeshComp->GetOwner());

}

void UAnimNotifyState_MordathKick::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}

void UAnimNotifyState_MordathKick::OnHit(USkeletalMeshComponent* MeshComp)
{
}
