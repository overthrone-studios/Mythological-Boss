// Copyright Overthrone Studios 2019

#include "Components/StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultStamina = 100.0f;

	InitStamina();
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	
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
