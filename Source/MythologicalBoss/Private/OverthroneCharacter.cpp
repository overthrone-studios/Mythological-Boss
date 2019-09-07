// Copyright Overthrone Studios 2019

#include "OverthroneCharacter.h"

#include "Public/OverthroneGameInstance.h"
#include "Public/OverthroneAnimInstance.h"
#include "Public/OverthroneHUD.h"

#include "Log.h"

#include "HUD/FSMVisualizerHUD.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/HealthComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "TimerManager.h"

AOverthroneCharacter::AOverthroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("Health Component"));

	GetCapsuleComponent()->bReturnMaterialOnMove = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	bCanBeDamaged = true;
}

void AOverthroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitTimeline(HealthLossTimeline, HealthLossCurve, 1.0f, FName("LoseHealth"), FName("FinishLosingHealth"));

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

void AOverthroneCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	HealthLossTimeline.TickTimeline(DeltaSeconds);

	if (IsMovingInAnyDirection() && !IsAttacking())
		CurrentMovementSpeed = MovementComponent->MaxWalkSpeed;
	else
		CurrentMovementSpeed = 0.0f;
}

void AOverthroneCharacter::UpdateCharacterInfo()
{
	check(0 && "You must implement UpdateCharacterInfo()");
}

float AOverthroneCharacter::GetWalkSpeed() const
{
	return MovementComponent->MaxWalkSpeed;
}

void AOverthroneCharacter::InitTimeline(FTimeline& InTimeline, UCurveFloat* InCurveFloat, const float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName)
{
	// Timeline Initialization
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, TimelineCallbackFuncName);
	TimelineFinishedCallback.BindUFunction(this, TimelineFinishedCallbackFuncName);

	if (InCurveFloat)
	{
		InTimeline.SetLooping(false);
		InTimeline.SetPlayRate(InPlaybackSpeed);
		InTimeline.AddInterpFloat(InCurveFloat, TimelineCallback);
		InTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		InTimeline.SetTimelineLength(InCurveFloat->FloatCurve.Keys[InCurveFloat->FloatCurve.Keys.Num() - 1].Time);
		InTimeline.SetTimelineLengthMode(TL_TimelineLength);
	}
	else
	{
		ULog::DebugMessage(ERROR, FString("Failed to initialize timeline. A curve float asset is missing!"), true);
	}
}

void AOverthroneCharacter::ApplyDamage(float DamageAmount)
{
	check(0 && "You must implement ApplyDamage()");
}

void AOverthroneCharacter::BeginTakeDamage(float DamageAmount)
{
	check(0 && "You must implement BeginTakeDamage()");
}

void AOverthroneCharacter::EndTakeDamage()
{
	check(0 && "You must implement EndTakeDamage()");
}

void AOverthroneCharacter::ApplyHitStop()
{

}

void AOverthroneCharacter::EnableInvincibility()
{
	bCanBeDamaged = false;
}

void AOverthroneCharacter::DisableInvincibility()
{
	bCanBeDamaged = true;
}

void AOverthroneCharacter::ChangeHitboxSize(float NewRadius)
{
	check(0 && "You must implement ChangeHitboxSize()");
}

void AOverthroneCharacter::OnLowHealth()
{
}

void AOverthroneCharacter::StartLosingHealth()
{
	HealthLossTimeline.PlayFromStart();
}

void AOverthroneCharacter::LoseHealth()
{
	const float Alpha = HealthLossCurve->GetFloatValue(HealthLossTimeline.GetPlaybackPosition());

	HealthComponent->SetSmoothedHealth(FMath::Lerp(HealthComponent->GetPreviousHealth(), HealthComponent->GetCurrentHealth(), Alpha));

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
	HealthComponent->UpdatePreviousHealth();

	// Stop animating displayed health
	if (HealthLossTimeline.IsPlaying())
		HealthLossTimeline.Stop();

	DecreaseHealth(HealthToSubtract);

	if (HealthComponent->GetDecreaseDelay() > 0.0f)
	{
		GetWorldTimerManager().SetTimer(HealthComponent->GetDelayTimerHandle(), this, &AOverthroneCharacter::StartLosingHealth, HealthComponent->GetDecreaseDelay(), false);
	}
	else
		StartLosingHealth();
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

bool AOverthroneCharacter::IsAttacking() const
{
	return false;
}

bool AOverthroneCharacter::IsMovingInAnyDirection() const
{
	return !GetVelocity().IsZero();
}
