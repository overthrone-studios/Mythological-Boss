// Copyright Overthrone Studios 2019

#include "Components/StaminaComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultStamina = 100.0f;

	InitStamina();
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	InitStamina();
}

void UStaminaComponent::DelayRegeneration()
{
	Owner->GetWorldTimerManager().SetTimer(RegenTimerHandle, RegenerationDelay, false);
}

bool UStaminaComponent::IsRegenerating()
{
	return !Owner->GetWorldTimerManager().IsTimerActive(RegenTimerHandle);
}

void UStaminaComponent::SetStamina(const float NewStaminaAmount)
{
	PreviousStamina = Stamina;

	Stamina = FMath::Clamp(NewStaminaAmount, 0.0f, DefaultStamina);
	NewStamina = Stamina;
}

void UStaminaComponent::SetSmoothedStamina(const float Value)
{
	NewStamina = Value;
}

void UStaminaComponent::IncreaseStamina(const float Amount)
{
	PreviousStamina = Stamina;

	Stamina = FMath::Clamp(Stamina + Amount, 0.0f, DefaultStamina);
	NewStamina = Stamina;
}

void UStaminaComponent::DecreaseStamina(const float Amount)
{
	PreviousStamina = Stamina;

	Stamina = FMath::Clamp(Stamina - Amount, 0.0f, DefaultStamina);
	NewStamina = Stamina;
}

void UStaminaComponent::ResetStamina()
{
	InitStamina();
}

void UStaminaComponent::InitStamina()
{
	Stamina = DefaultStamina;
	PreviousStamina = DefaultStamina;
	NewStamina = DefaultStamina;
}

#if WITH_EDITOR
void UStaminaComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "DefaultStamina")
		Stamina = DefaultStamina;
}
#endif
