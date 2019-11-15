// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TipMessageData.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UTipMessageData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Tip Message Data")
		FText TipMessage = FText::FromString("None");

	UPROPERTY(EditInstanceOnly, Category = "Tip Message Data")
		FName ActionName = "None";
};
