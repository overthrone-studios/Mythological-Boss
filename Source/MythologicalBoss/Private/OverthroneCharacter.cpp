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

#include "FSM.h"

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
	
	UOverthroneFunctionLibrary::SetupTimeline(this, TL_HealthLoss, HealthLossCurve, false, 1.0f, FName("LoseHealth"), FName("FinishLosingHealth"));
	UOverthroneFunctionLibrary::SetupTimeline(this, TL_Knockback, KnockbackCurve, false, 1.0f, FName("DoKnockback"), FName("OnFinishedKnockback"));

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
	OverthroneHUD = Cast<AOverthroneHUD>(PlayerController->GetHUD());
	MainHUD = OverthroneHUD->GetMainHUD();
	GameInstance = UOverthroneFunctionLibrary::GetGameInstance(this);
	GameState = UOverthroneFunctionLibrary::GetGameState(this);
	SKMComponent = GetMesh();
	CapsuleComp = GetCapsuleComponent();
}

void AOverthroneCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TL_HealthLoss.TickTimeline(DeltaSeconds);
	TL_Knockback.TickTimeline(DeltaSeconds);

	CurrentLocation = GetActorLocation();
	CurrentRotation = GetActorRotation();

	ForwardVector = GetActorForwardVector();
	RightVector = GetActorRightVector();

	#if !UE_BUILD_SHIPPING
	if (IsLocked())
		return;
	#endif

	if (IsMovingInAnyDirection() && !IsAttacking())
		CurrentMovementSpeed = GetMovementSpeed();
	else
		CurrentMovementSpeed = 0.0f;

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

void AOverthroneCharacter::BeginTakeDamage(const float DamageAmount, const FDamageEvent& DamageEvent)
{
	RecentDamage = DamageAmount;
}

void AOverthroneCharacter::EndTakeDamage(const FDamageEvent& DamageEvent)
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

void AOverthroneCharacter::OnAttackLanded(const FHitResult& HitResult)
{
}

void AOverthroneCharacter::ChangeHitboxSize(float NewRadius)
{
	check(0 && "You must implement ChangeHitboxSize()");
}

#pragma region Overthrone Character Base States
#pragma region Death
void AOverthroneCharacter::OnEnterDeathState()
{
	AnimInstance->StopAllMontages(0.4f);

	bIsDead = true;
	AnimInstance->bIsDead = true;

	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	MovementComponent->DisableMovement();
}

void AOverthroneCharacter::UpdateDeathState(float Uptime, int32 Frames)
{
	StopMovement();

	if (AnimInstance->AnimTimeRemaining < 0.1f)
		FSM->Stop();
}

void AOverthroneCharacter::OnExitDeathState()
{
	bIsDead = true;
	AnimInstance->bIsDead = true;
}
#pragma endregion

#pragma region Locked
void AOverthroneCharacter::OnEnterLockedState()
{
	StopAnimMontage();
}

void AOverthroneCharacter::UpdateLockedState(float Uptime, int32 Frames)
{
	StopMovement();
}

void AOverthroneCharacter::OnExitLockedState()
{
}
#pragma endregion
#pragma endregion

void AOverthroneCharacter::DoKnockback()
{
}

void AOverthroneCharacter::OnFinishedKnockback()
{
}

void AOverthroneCharacter::OnLowHealth()
{
}

void AOverthroneCharacter::OnExitLowHealth()
{
}

void AOverthroneCharacter::StartLosingHealth()
{
	TL_HealthLoss.PlayFromStart();
}

void AOverthroneCharacter::LoseHealth()
{
	const float Alpha = HealthLossCurve->GetFloatValue(TL_HealthLoss.GetPlaybackPosition());

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
	if (TL_HealthLoss.IsPlaying())
		TL_HealthLoss.Stop();

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

	UpdateHealth(HealthComponent->GetCurrentHealth());
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

void AOverthroneCharacter::ApplyKnockbackEffect()
{
}

bool AOverthroneCharacter::IsInvincible() const
{
	return !bCanBeDamaged;
}

void AOverthroneCharacter::StopMovement()
{
	CurrentMovementSpeed = 0.0f;
	ForwardInput = 0.0f;
	RightInput = 0.0f;

	AnimInstance->MovementSpeed = 0.0f;
	AnimInstance->MovementDirection = 0.0f;
	AnimInstance->ForwardInput = 0.0f;
	AnimInstance->RightInput = 0.0f;
}

void AOverthroneCharacter::ToggleLockSelf()
{
	check(0 && "You must implement ToggleLockSelf()");
}

void AOverthroneCharacter::LockSelf()
{
	check(0 && "You must implement LockSelf()");
}

void AOverthroneCharacter::UnlockSelf()
{
	check(0 && "You must implement UnlockSelf()");
}

bool AOverthroneCharacter::IsLocked() const
{
	return bIsLocked;
}

bool AOverthroneCharacter::IsHealthZero() const
{
	return HealthComponent->GetCurrentHealth() <= 0.0f;
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

float AOverthroneCharacter::GetRecentDamage() const
{
	return RecentDamage;
}

bool AOverthroneCharacter::IsAttacking() const
{
	return false;
}

bool AOverthroneCharacter::IsMovingInAnyDirection() const
{
	return ForwardInput != 0.0f || RightInput != 0.0f;
}

bool AOverthroneCharacter::IsDead() const
{
	return bIsDead;
}
