// Copyright Overthrone Studios 2019

#pragma once

#include "GameFramework/HUD.h"
#include "OverthroneHUD.generated.h"

USTRUCT()
struct FDebugData
{
	GENERATED_BODY()

	FString Message;

	FLinearColor Color;

	float XOffset;
	float YOffset;
};

/**
 * The main HUD the player will use when playing the game
 */
UCLASS()
class MYTHOLOGICALBOSS_API AOverthroneHUD final : public AHUD
{
	GENERATED_BODY()

public:
	AOverthroneHUD();

	void Init();

	FORCEINLINE class UMasterHUD* GetMasterHUD() const { return MasterHUD; }
	class UMainPlayerHUD* GetMainHUD() const;

	void AddOnScreenDebugMessage(const FString& Message, FLinearColor Color, float YPadding = 0.0f);
	void UpdateOnScreenDebugMessage(int32 Index, const FString& Message);

	uint8 bHideDebugText : 1;

	FORCEINLINE int32 GetDebugMessagesCount() const { return DebugMessages.Num(); }

protected:
	void DrawHUD() override;

	void CreateWidgets();
	void AddWidgetsToScreen();
	void InitWidgets();

	TArray<FDebugData> DebugMessages;

	class UMasterHUD* MasterHUD{};
	TSubclassOf<class UHUDBase> HUDWidgetClass;

	float BaseXOffset = 220.0f;

	float NewYOffset = 30.0f;
	float YOffsetInterval = 15.0f;

private:
	FVector2D ViewportSize;
};
