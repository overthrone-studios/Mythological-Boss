// Copyright Overthrone Studios 2019

#include "AnimNotify_PlayForceFeedback.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Log.h"

void UAnimNotify_PlayForceFeedback::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// Don't call super to avoid call back in to blueprints

	if (ForceFeedback)
		UGameplayStatics::SpawnForceFeedbackAtLocation(MeshComp, ForceFeedback, MeshComp->GetSocketLocation(Bone), FRotator::ZeroRotator);
	else
		ULog::DebugMessage(WARNING, Animation->GetName() + ": Force Feedback asset not found.", true);
}
