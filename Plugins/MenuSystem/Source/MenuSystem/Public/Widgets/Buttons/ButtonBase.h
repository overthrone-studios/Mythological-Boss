// Copyright Ali El Saleh 2019

#pragma once

#include "Blueprint/UserWidget.h"
#include "ButtonBase.generated.h"

UENUM()
enum EButtonType
{
	BTN_NEW_GAME	UMETA(DisplayName = "New game"),
	BTN_LVL_SELECT	UMETA(DisplayName = "Level Select"),
	BTN_FEATS		UMETA(DisplayName = "Feats"),
	BTN_OPTIONS		UMETA(DisplayName = "Options"),
	BTN_CREDITS		UMETA(DisplayName = "Credits"),
	BTN_VIDEO		UMETA(DisplayName = "Video"),
	BTN_AUDIO		UMETA(DisplayName = "Audio"),
	BTN_CONTROLS	UMETA(DisplayName = "Controls"),
	BTN_RESET_INPUT	UMETA(DisplayName = "Reset input bindings"),
	BTN_APPLY		UMETA(DisplayName = "Apply"),
	BTN_DISCARD		UMETA(DisplayName = "Discard"),
	BTN_RESET		UMETA(DisplayName = "Reset"),
	BTN_RESET_YES	UMETA(DisplayName = "Reset confirm Yes"),
	BTN_RESET_NO	UMETA(DisplayName = "Reset confirm No"),
	BTN_BACK		UMETA(DisplayName = "Back"),
	BTN_EXIT		UMETA(DisplayName = "Exit"),
	BTN_NONE		UMETA(DisplayName = "None")
};

/**
 * Base class for a menu button
 */
UCLASS()
class MENUSYSTEM_API UButtonBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init();

protected:
	void NativePreConstruct() override;

	// For button functionality
	UFUNCTION(BlueprintCallable, Category = "Events")
		virtual void OnButtonReleased();

	// For tooltip text change
	UFUNCTION(BlueprintCallable, Category = "Events")
		virtual void OnButtonHovered();
	UFUNCTION(BlueprintCallable, Category = "Events")
		virtual void OnButtonUnhovered();

	virtual void HighlightText();
	virtual void UnHighlightText();

	UPROPERTY(EditInstanceOnly, Category = "Menu Button")
		TEnumAsByte<EButtonType> ButtonType;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button")
		FText ButtonText;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button")
		FText ButtonTooltipText;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button", meta = (ClampMin = 0.0f, ClampMax=1.0f))
		float HoveredOpacity = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button", meta = (ClampMin = 0.0f, ClampMax=1.0f))
		float UnhoveredOpacity = 0.8f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button")
		USoundBase* PressedSound;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button")
		USoundBase* HoveredSound;

	// Is this a menu button that controls menu switching?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button")
		uint8 bIsMenuSwitchButton : 1;

	// The menu to go switch to
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Menu Button", meta = (EditCondition = "bIsMenuSwitchButton"))
		int32 MenuIndex = 0;

	class UTextBlock* TextWidget;

	class AMenuHUD* MenuHUD;

	class UMenuBase* Menu;

	bool IsMenuNull() const;

private:
	bool IsMenuHUDNull() const;
	bool IsTextWidgetNull() const;
};
