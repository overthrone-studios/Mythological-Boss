// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MordathFeatherComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYTHOLOGICALBOSS_API UMordathFeatherComponent final : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UMordathFeatherComponent();
	
	void RevertMaterial();

protected:
	void BeginPlay() override;

private:
	class UMaterialInterface* OriginalMaterial;
	class UMaterialInstanceDynamic* MID_OriginalMaterial;
};
