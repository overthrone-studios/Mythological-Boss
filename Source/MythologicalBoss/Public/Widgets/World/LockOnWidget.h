// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOnWidget.generated.h"

/**
 * Controls lock-on widget logic
 */
UCLASS()
class MYTHOLOGICALBOSS_API ULockOnWidget final : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "LockOn Widget")
		class UWidgetAnimation* Fade;
};
