// Copyright Overthrone Studios 2019

#include "TeleportationComponent.h"

#include "OverthroneFunctionLibrary.h"
#include "BoundingBox.h"

#include "Curves/CurveFloat.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/Character.h"

#include "Kismet/KismetMathLibrary.h"

#include "TimerManager.h"

#include "DrawDebugHelpers.h"
#include "Log.h"

UTeleportationComponent::UTeleportationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTeleportationComponent::Disappear()
{
	if (!TL_Dissolve.IsPlaying())
	{
		MID_Dissolve = SKMComponent->CreateDynamicMaterialInstance(0, OriginalMaterial, FName("MID_Dissolve"));
		MID_Dissolve->SetScalarParameterValue("IsDissolving", 1.0f);

		TL_Dissolve.Stop();
		TL_Dissolve.SetPlaybackPosition(0.0f, true, true);

		OnBeginDisappear.Broadcast();

		TL_Dissolve.PlayFromStart();
	}
}

void UTeleportationComponent::Reappear()
{
	//if (!TL_Dissolve.IsPlaying())
	//{
		MID_Dissolve = SKMComponent->CreateDynamicMaterialInstance(0, OriginalMaterial, FName("MID_Dissolve"));
		MID_Dissolve->SetScalarParameterValue("IsDissolving", 1.0f);

		OnBeginReappear.Broadcast();

		bWasReversing = true;
		TL_Dissolve.ReverseFromEnd();
	//}
}

void UTeleportationComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	SKMComponent = Owner->GetMesh();
	TimerManager = &Owner->GetWorldTimerManager();

	OriginalMaterial = SKMComponent->GetMaterial(0);

	UOverthroneFunctionLibrary::SetupTimeline(this, TL_Dissolve, DissolveCurve, false, DissolveSpeed, "UpdateDissolve", "FinishDissolve");
}

void UTeleportationComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TL_Dissolve.TickTimeline(DeltaTime);
}

void UTeleportationComponent::UpdateDissolve()
{
	const float Time = TL_Dissolve.GetPlaybackPosition();
	const float DissolveAmount = DissolveCurve->GetFloatValue(Time);

	MID_Dissolve->SetScalarParameterValue("Amount", DissolveAmount);
}

void UTeleportationComponent::FinishDissolve()
{
	if (bWasReversing)
	{
		bWasReversing = false;

		SKMComponent->SetMaterial(0, OriginalMaterial);

		OnReappeared.Broadcast();
	}
	else
	{
		OnDisappeared.Broadcast();
	}
}

void UTeleportationComponent::GenerateTeleportTime()
{
	const float Min = TeleportTime - RandomDeviation;
	const float Max = TeleportTime + RandomDeviation;
	const float NewTime = FMath::FRandRange(Min, Max);

	TeleportTime = NewTime;

	if (bLogTeleportTime)
		ULog::Number(TeleportTime, "Teleport time: ", true);
}

void UTeleportationComponent::StartCooldown()
{
	TimerManager->SetTimer(TH_Cooldown, CooldownTime, false);
}

bool UTeleportationComponent::IsCoolingDown()
{
	return TimerManager->IsTimerActive(TH_Cooldown);
}

FVector UTeleportationComponent::FindLocationToTeleport(const FVector& Origin, const float Radius, const ABoundingBox* InPlayArea) const
{
	const float T = FMath::RandRange(0.0f, 2 * PI);

	FVector NewLocation
	{
		Origin.X + Radius * FMath::Cos(T), 
		Origin.Y + Radius * FMath::Sin(T), 
		Owner->GetActorLocation().Z
	};

	// If the calculated location is outside the box
	if (!UKismetMathLibrary::IsPointInBox(NewLocation, InPlayArea->GetActorLocation(), InPlayArea->GetBoundingBox().GetExtent()))
	{
		NewLocation = UKismetMathLibrary::RandomPointInBoundingBox(InPlayArea->GetActorLocation(), InPlayArea->GetBoundingBox().GetExtent());
	}

	if (bShowTeleportedLocation)
	{
		// New teleported location
		DrawDebugLine(GetWorld(), NewLocation, NewLocation * FVector(1.0f, 1.0f, 3000.0f), FColor::Red, true, 5.0f, 0, 2.0f);

		// The origin location
		DrawDebugLine(GetWorld(), Origin, Origin * FVector(1.0f, 1.0f, 3000.0f), FColor::Green, true, 5.0f, 0, 2.0f);

		// The generated location visualized as a wire sphere
		DrawDebugSphere(GetWorld(), NewLocation, 20.0f, 20, FColor::Green, false, 2.0f, 0.0f, 3.0f);
	}

	return FVector(NewLocation.X, NewLocation.Y, Owner->GetActorLocation().Z);
}
