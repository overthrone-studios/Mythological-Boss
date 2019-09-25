// Copyright Overthrone Studios 2019

#include "YlvaGhost.h"

#include "OverthroneFunctionLibrary.h"
#include "OverthroneCharacter.h"

#include "Components/PoseableMeshComponent.h"
#include "Components/TimelineComponent.h"

#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Curves/CurveFloat.h"

#include "Kismet/KismetMaterialLibrary.h"

#include "Log.h"

AYlvaGhost::AYlvaGhost()
{
	PrimaryActorTick.bCanEverTick = false;

	PoseableMeshComponent = CreateDefaultSubobject<UPoseableMeshComponent>("Poseable Mesh Component");

	TL_Opacity = CreateDefaultSubobject<UTimelineComponent>("Opacity Timeline");
}

void AYlvaGhost::BeginPlay()
{
	Super::BeginPlay();

	Ylva = UOverthroneFunctionLibrary::GetPlayerCharacter(this);

	InitTimeline();

	CreateGhost();
}

void AYlvaGhost::CreateGhost()
{
	const TArray<UMaterialInterface*>& Materials = PoseableMeshComponent->GetMaterials();
	for (int32 i = 0; i < Materials.Num(); i++)
	{
		UMaterialInstanceDynamic* MID = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, GhostMaterialInstance);

		const int32& Index = MIDs.Add(MID);

		PoseableMeshComponent->SetMaterial(Index, Cast<UMaterialInterface>(MID));
		PoseableMeshComponent->CopyPoseFromSkeletalComponent(Ylva->GetMesh());

		TL_Opacity->PlayFromStart();
	}
}

void AYlvaGhost::FadeOut()
{
	const float& PlaybackPosition = TL_Opacity->GetPlaybackPosition();
	const float& OpacityCurveValue = OpacityCurve->GetFloatValue(PlaybackPosition);

	for (const auto& MID : MIDs)
	{
		MID->SetScalarParameterValue("Opacity", OpacityCurveValue);
	}
}

void AYlvaGhost::DestroySelf()
{
	Destroy();
}

void AYlvaGhost::InitTimeline()
{
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, "FadeOut");
	TimelineFinishedCallback.BindUFunction(this, "DestroySelf");

	if (OpacityCurve)
	{
		TL_Opacity->SetLooping(false);
		TL_Opacity->SetPlayRate(1.0f);
		TL_Opacity->AddInterpFloat(OpacityCurve, TimelineCallback);
		TL_Opacity->SetTimelineFinishedFunc(TimelineFinishedCallback);
		TL_Opacity->SetTimelineLength(OpacityCurve->FloatCurve.Keys[OpacityCurve->FloatCurve.Keys.Num() - 1].Time);
		TL_Opacity->SetTimelineLengthMode(TL_TimelineLength);
	}
	else
	{
		ULog::DebugMessage(ERROR, FString("Failed to initialize timeline. A curve float asset is missing!"), true);
	}
}
