// Copyright Overthrone Studios 2019

#include "OverthroneCharacter.h"
#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneAnimInstance.h"
#include "Public/OverthroneHUD.h"
#include "HUD/FSMVisualizerHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Log.h"
#include "Components/TimelineComponent.h"
#include "Components/HealthComponent.h"
#include "TimerManager.h"

AOverthroneCharacter::AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("Health Component"));

	// Take damage timeline component
	HealthLossTimeline = CreateDefaultSubobject<UTimelineComponent>(FName("Take Damage Timeline"));

	GetCapsuleComponent()->bReturnMaterialOnMove = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	bCanBeDamaged = true;
}

void AOverthroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitTimelineComponent(HealthLossTimeline, HealthLossCurve, 1.0f, FName("LoseHealth"), FName("FinishLosingHealth"));

	// Store all our child components
	Components = GetComponents();

	// Become vulnerable
	bCanBeDamaged = true;

	// Initialize variables
	World = GetWorld();
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	MovementComponent = GetCharacterMovement();
	AnimInstance = Cast<UOverthroneAnimInstance>(GetMesh()->GetAnimInstance());
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	GameInstance = Cast<UOverthroneGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void AOverthroneCharacter::UpdateCharacterInfo()
{
	check(0 && "You must implement UpdateCharacterInfo()");
}

void AOverthroneCharacter::InitTimelineComponent(UTimelineComponent* InTimelineComponent, UCurveFloat* InCurveFloat, const float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName)
{
	// Timeline Initialization
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, TimelineCallbackFuncName);
	TimelineFinishedCallback.BindUFunction(this, TimelineFinishedCallbackFuncName);

	if (InCurveFloat)
	{
		InTimelineComponent = NewObject<UTimelineComponent>(this, InTimelineComponent->GetFName());
		InTimelineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		InTimelineComponent->SetPropertySetObject(this);
		InTimelineComponent->SetLooping(false);
		InTimelineComponent->SetPlaybackPosition(0.0f, false, false);
		InTimelineComponent->SetPlayRate(InPlaybackSpeed);
		InTimelineComponent->AddInterpFloat(InCurveFloat, TimelineCallback);
		InTimelineComponent->SetTimelineFinishedFunc(TimelineFinishedCallback);
		InTimelineComponent->SetTimelineLength(InCurveFloat->FloatCurve.Keys[InCurveFloat->FloatCurve.Keys.Num() - 1].Time);
		InTimelineComponent->SetTimelineLengthMode(TL_TimelineLength);
		InTimelineComponent->RegisterComponent();
	}
	else
	{
		ULog::DebugMessage(ERROR, "Failed to initialize the " + InTimelineComponent->GetName() + ". A curve float asset is missing!", true);
	}
}

void AOverthroneCharacter::ApplyHitStop()
{

}

void AOverthroneCharacter::ChangeHitboxSize(float NewRadius)
{
	check(0 && "You must implement ChangeHitboxSize()");
}

void AOverthroneCharacter::OnLowHealth()
{
}

void AOverthroneCharacter::StartLosingHealth(const float Amount)
{
	HealthLossTimeline->PlayFromStart();
}

void AOverthroneCharacter::LoseHealth()
{
	const float Time = HealthLossCurve->GetFloatValue(HealthLossTimeline->GetPlaybackPosition());

	HealthComponent->SetSmoothedHealth(FMath::Lerp(HealthComponent->GetPreviousHealth(), HealthComponent->GetCurrentHealth(), Time));

	UpdateCharacterInfo();
	
	// Are we on low health?
	if (HealthComponent->IsLowHealth() && !bWasLowHealthEventTriggered)
		BroadcastLowHealth();
}

void AOverthroneCharacter::FinishLosingHealth()
{
	UpdateCharacterInfo();
}

void AOverthroneCharacter::SetHealth(const float NewHealthAmount)
{
	HealthComponent->SetHealth(NewHealthAmount);

	UpdateCharacterInfo();

	// Are we on low health?
	if (HealthComponent->IsLowHealth() && !bWasLowHealthEventTriggered)
		BroadcastLowHealth();
}

void AOverthroneCharacter::IncreaseHealth(const float Amount)
{
	HealthComponent->IncreaseHealth(Amount);

	UpdateCharacterInfo();
}

void AOverthroneCharacter::DecreaseHealth(const float Amount)
{
	HealthComponent->DecreaseHealth(Amount);

	UpdateCharacterInfo();

	// Are we on low health?
	if (HealthComponent->IsLowHealth() && !bWasLowHealthEventTriggered)
		BroadcastLowHealth();
}

void AOverthroneCharacter::ResetHealth()
{
	HealthComponent->ResetHealth();

	UpdateCharacterInfo();
}

void AOverthroneCharacter::UpdateHealth(const float HealthToSubtract)
{
	if (HealthComponent->IsUsingSmoothBar())
	{
		DecreaseHealth(HealthToSubtract);

		if (HealthComponent->GetDecreaseDelay() > 0.0f)
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, "StartLosingHealth", HealthToSubtract);
			GetWorldTimerManager().SetTimer(HealthComponent->GetDelayTimerHandle(), TimerDelegate, HealthComponent->GetDecreaseDelay(), false);
		}
		else
			StartLosingHealth(HealthToSubtract);
	}
	else
		DecreaseHealth(HealthToSubtract);
}

void AOverthroneCharacter::Die()
{
	bCanBeDamaged = false;

	AnimInstance->LeaveAllStates();
}

void AOverthroneCharacter::BroadcastLowHealth()
{
	check(0 && "You must implement BroadcastLowHealth()");
}

void AOverthroneCharacter::PauseAnims() const
{
	GetMesh()->bPauseAnims = true;
}

void AOverthroneCharacter::UnPauseAnims() const
{
	GetMesh()->bPauseAnims = false;
}

bool AOverthroneCharacter::IsInvincible() const
{
	return !bCanBeDamaged;
}
