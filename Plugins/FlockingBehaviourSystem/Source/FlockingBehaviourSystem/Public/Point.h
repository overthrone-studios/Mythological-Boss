// Copyright Ali El Saleh, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "Point.generated.h"

UCLASS(HideCategories = ("Rendering", "Input", "Actor", "LOD", "Cooking"))
class FLOCKINGBEHAVIOURSYSTEM_API APoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APoint();

	void SetVisibility(bool bIsVisible) const;
	void SetOwningGoal(class AGoal* Goal);
	class AGoal* GetGoalOwner() const;

	uint32 ID = 1;

protected:
	void BeginPlay() override;

	#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UBillboardComponent* BillboardComponent;
	#pragma endregion

	#pragma region Properties
	UPROPERTY(VisibleAnywhere, Category = "Properties", DisplayName = "Owned By")
		class AGoal* OwningGoal{};

	UPROPERTY(EditAnywhere, Category = "Properties", meta = (ToolTip = "Show the billboard in editor?"))
		bool bVisible = true;
	#pragma endregion

private:
	// The sprite that can be seen in the editor
	UTexture2D* BillboardSprite;
};
