// Copyright Overthrone Studios 2019

#include "Components/HealthComponent.h"
#include "OverthroneCharacter.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultHealth = 100.0f;

	InitHealth();
}

bool UHealthComponent::IsDelayFinished()
{
	return !Owner->GetWorldTimerManager().IsTimerActive(DelayTimerHandle);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	InitHealth();

	// Retrieve the owner of this component and bind our function to the OnTakeAnyDamage event
	Owner = GetOwner();
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

#if WITH_EDITOR
void UHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// If default health value has changed, up current health
	if (PropertyChangedEvent.GetPropertyName() == "DefaultHealth")
		Health = DefaultHealth;
}
#endif

void UHealthComponent::InitHealth()
{
	Health = DefaultHealth;
	PreviousHealth = DefaultHealth;
	SmoothedHealth = DefaultHealth;
}

void UHealthComponent::SetDefaultHealth(const float NewDefaultHealth)
{
	DefaultHealth = NewDefaultHealth;
}

void UHealthComponent::SetHealth(const float NewHealthAmount)
{
	PreviousHealth = Health;

	Health = FMath::Clamp(NewHealthAmount, 0.0f, DefaultHealth);
	SmoothedHealth = Health;
}

void UHealthComponent::SetSmoothedHealth(const float Value)
{
	SmoothedHealth = Value;
}

void UHealthComponent::UpdatePreviousHealth()
{
	PreviousHealth = SmoothedHealth;
}

void UHealthComponent::IncreaseHealth(const float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, DefaultHealth);
	SmoothedHealth = Health;
}

void UHealthComponent::DecreaseHealth(const float Amount)
{
	if (SmoothedHealth <= Health)
		PreviousHealth = Health;

	Health = FMath::Clamp(Health - Amount, 0.0f, DefaultHealth);
}

void UHealthComponent::ResetHealth()
{
	InitHealth();
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, const float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
		return;

	Owner->TakeDamage(Damage, FDamageEvent(), InstigatedBy, DamageCauser);
}
