// Copyright Overthrone Studios 2019

#include "OverthroneCharacter.h"

#include "Public/OverthroneGameState.h"
#include "Public/OverthroneAnimInstance.h"
#include "Public/OverthroneHUD.h"
#include "Public/OverthroneFunctionLibrary.h"

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
	
	UOverthroneFunctionLibrary::SetupTimeline(this, HealthLossTimeline, HealthLossCurve, false, 1.0f, FName("LoseHealth"), FName("FinishLosingHealth"));

	// Store all our child components
	Components = GetComponents();

	// Become vulnerable
	bCanBeDamaged = true;

	// Initialize variables
	World = GetWorld();
	TimerManager = &World->GetTimerManager();
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	MovementComponent = GetCharacterMovement();
	AnimInstance = Cast<UOverthroneAnimInstance>(GetMesh()->GetAnimInstance());
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	MainHUD = OverthroneHUD->GetMainHUD();
	GameInstance = UOverthroneFunctionLibrary::GetGameInstance(this);
	GameState = UOverthroneFunctionLibrary::GetGameState(this);
	SKMComponent = GetMesh();
	CapsuleComp = GetCapsuleComponent();
}

void AOverthroneCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	HealthLossTimeline.TickTimeline(DeltaSeconds);

	if (IsLocked())
		return;

	if (IsMovingInAnyDirection() && !IsAttacking())
		CurrentMovementSpeed = GetMovementSpeed();
	else
		CurrentMovementSpeed = 0.0f;

	CurrentLocation = GetActorLocation();
	CurrentRotation = GetActorRotation();

	ForwardVector = GetActorForwardVector();
	RightVector = GetActorRightVector();

	ControlRotation = GetControlRotation();
}

void AOverthroneCharacter::UpdateCharacterInfo()
{
	check(0 && "You must implement UpdateCharacterInfo()");
}

void AOverthroneCharacter::AddDebugMessages()
{
	check(0 && "You must implement AddDebugMessages()");
}

float AOverthroneCharacter::GetMovementSpeed() const
{
	return MovementComponent->MaxWalkSpeed;
}

void AOverthroneCharacter::ApplyDamage(float DamageAmount, const FDamageEvent& DamageEvent)
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

void AOverthroneCharacter::OnAttackLanded(FHitResult& HitResult)
{
}

void AOverthroneCharacter::ChangeHitboxSize(float NewRadius)
{
	check(0 && "You must implement ChangeHitboxSize()");
}

void AOverthroneCharacter::OnLowHealth()
{
}

void AOverthroneCharacter::OnExitLowHealth()
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

	if (!HealthComponent->IsLowHealth() && bWasLowHealthEventTriggered)
		BroadcastExitLowHealth();

	// Are we on low health?
	if (HealthComponent->IsLowHealth() && !bWasLowHealthEventTriggered)
		BroadcastLowHealth();
}

void AOverthroneCharacter::IncreaseHealth(const float Amount)
{
	HealthComponent->IncreaseHealth(Amount);

	UpdateCharacterInfo();

	if (!HealthComponent->IsLowHealth() && bWasLowHealthEventTriggered)
		BroadcastExitLowHealth();
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

	if (!HealthComponent->IsLowHealth() && bWasLowHealthEventTriggered)
		BroadcastExitLowHealth();
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

void AOverthroneCharacter::BroadcastExitLowHealth()
{
	bWasLowHealthEventTriggered = false;
}

void AOverthroneCharacter::PauseAnims() const
{
	SKMComponent->bPauseAnims = true;
}

void AOverthroneCharacter::UnPauseAnims() const
{
	SKMComponent->bPauseAnims = false;
}

bool AOverthroneCharacter::IsInvincible() const
{
	return !bCanBeDamaged;
}

bool AOverthroneCharacter::HasMovedRightBy(float Distance)
{
	check(0 && "You must implement HasMovedRightBy()");
	return false;
}

bool AOverthroneCharacter::HasMovedLeftBy(float Distance)
{
	check(0 && "You must implement HasMovedLeftBy()");
	return false;
}

void AOverthroneCharacter::ToggleLockSelf()
{
	check(0 && "You must implement ToggleLockSelf()");
}

bool AOverthroneCharacter::IsLocked() const
{
	return bIsLocked;
}

void AOverthroneCharacter::VibrateController(UForceFeedbackEffect* ForceFeedbackToPlay, const bool bLooping, const bool bIgnoreTimeDilation)
{
	if (ForceFeedbackToPlay)
		PlayerController->ClientPlayForceFeedback(ForceFeedbackToPlay, bLooping, bIgnoreTimeDilation, ForceFeedbackToPlay->GetFName());
}

void AOverthroneCharacter::VibrateController( const float Intensity, const float Duration, const bool bLeftSide, const bool bRightSide)
{
	PlayerController->PlayDynamicForceFeedback(Intensity, Duration, bLeftSide, bLeftSide, bRightSide, bRightSide, EDynamicForceFeedbackAction::Start);
}

void AOverthroneCharacter::StopVibrateController(UForceFeedbackEffect* ForceFeedbackToStop)
{
	if (ForceFeedbackToStop)
		PlayerController->ClientStopForceFeedback(ForceFeedbackToStop, ForceFeedbackToStop->GetFName());
}

bool AOverthroneCharacter::IsAttacking() const
{
	return false;
}

bool AOverthroneCharacter::IsMovingInAnyDirection() const
{
	return !GetVelocity().IsZero();
}

bool AOverthroneCharacter::IsDead() const
{
	return bIsDead;
}
