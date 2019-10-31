// Copyright Overthrone Studios 2019

#include "TutorialData.h"
#include "OverthroneFunctionLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/InputSettings.h"

void UTutorialData::UpdateRequestedKey()
{
	const auto InputSettings = GetMutableDefault<UInputSettings>();
	TArray<FInputActionKeyMapping> ActionKeyMappings;
	InputSettings->GetActionMappingByName(ActionName, ActionKeyMappings);
	FKey Key;
	for (const auto KeyMapping : ActionKeyMappings)
	{
		Key = KeyMapping.Key;

		// Is KeyMapping a gamepad key?
		if (UOverthroneFunctionLibrary::IsGamepadConnected() && KeyMapping.Key.IsGamepadKey())
		{
			RequestedInputKey = KeyMapping.Key;
			return;
		}

		if (!UOverthroneFunctionLibrary::IsGamepadConnected() && !KeyMapping.Key.IsGamepadKey())
		{
			RequestedInputKey = KeyMapping.Key;
			return;
		}
	}

	RequestedInputKey = Key;
}

FText UTutorialData::GetTutorialProgress()
{
	return FText::FromString(FString::FromInt(CurrentCount) + "/" + FString::FromInt(MaxCount));
}

