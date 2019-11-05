// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GalleryData_Base.generated.h"

/**
 * An asset that can be used in the gallery menu
 */
UCLASS(BlueprintType)
class MYTHOLOGICALBOSS_API UGalleryData_Base : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// The person who created the asset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gallery Data")
		FName CreditName = "None";
};
