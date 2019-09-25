// Copyright Overthrone Studios 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YlvaGhost.generated.h"

UCLASS()
class MYTHOLOGICALBOSS_API AYlvaGhost final : public AActor
{
	GENERATED_BODY()
	
public:	
	AYlvaGhost();

protected:
	void BeginPlay() override;

	void CreateGhost();

	UFUNCTION()
		void FadeOut();

	UFUNCTION()
		void DestroySelf();

	UPROPERTY(EditDefaultsOnly)
		class UPoseableMeshComponent* PoseableMeshComponent;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Ghost Material")
		class UMaterialInstance* GhostMaterialInstance;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Opacity Timeline")
		class UCurveFloat* OpacityCurve;
		
private:
	void InitTimeline();

	TArray<UMaterialInstanceDynamic*> MIDs;

	class AOverthroneCharacter* Ylva;

	class UTimelineComponent* TL_Opacity;
};
