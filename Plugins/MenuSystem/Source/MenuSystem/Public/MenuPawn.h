// Copyright Ali El Saleh 2019

#pragma once

#include "GameFramework/Pawn.h"
#include "MenuPawn.generated.h"

UCLASS(HideCategories = ("Rendering", "Replication", "Actor", "Collision", "LOD", "Cooking"))
class MENUSYSTEM_API AMenuPawn final : public APawn
{
	GENERATED_BODY()

public:
	AMenuPawn();

protected:
	void BeginPlay() override;

	void PossessedBy(AController* NewController) override;

	// Switches input mode to UI only
	void EnableUIMode() const;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UCameraComponent* CameraComponent;

	APlayerController* PlayerController;
};
