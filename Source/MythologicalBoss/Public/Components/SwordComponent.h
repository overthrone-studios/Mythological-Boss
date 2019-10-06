// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SwordComponent.generated.h"

/**
 * A component that can be attached to a character
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Activation", "Cooking", "AssetUserData"))
class MYTHOLOGICALBOSS_API USwordComponent final : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	USwordComponent();

	// The material to use when the charge meter is full
	UPROPERTY(EditAnywhere, Category = "Sword")
		class UMaterialInterface* GlowMaterial;

	// Assign the glow material to the given material slot (specified in the editor)
	UFUNCTION(BlueprintCallable, Category = "Sword")
		void Glow();

	// Reverts back to the original sword material
	UFUNCTION(BlueprintCallable, Category = "Sword")
		void Revert();

protected:
	void BeginPlay() override;

	// The index to assign the glow material to
	UPROPERTY(EditAnywhere, Category = "Sword")
		uint8 MaterialIndex = 0;

private:
	class UMaterialInterface* DefaultSwordMaterial;

	AActor* Owner;
};
