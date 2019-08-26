// Copyright Ali El Saleh 2019

#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

UENUM()
enum EMenuType
{
	MAIN,
	OPTIONS,
	NEW_GAME,
	CONTROLS,
	VIDEO,
	AUDIO,
	CREDITS
};

/**
 * Base class for any type of menu widgets i.e (Main, Options, Video, Audio, Input, etc.)  
 */
UCLASS()
class MENUSYSTEM_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void FadeIn();
	UFUNCTION(BlueprintCallable)
	virtual void FadeOut();

	UFUNCTION(BlueprintCallable)
	virtual void Init();

	UFUNCTION(BlueprintCallable)
		virtual void Apply();

	UFUNCTION(BlueprintCallable)
		virtual void Back();

	UFUNCTION(BlueprintCallable)
		virtual void Reset();

	virtual void SetMenuTooltipText(const FText& Text);

	UFUNCTION(BlueprintCallable)
	virtual bool AreAllSettingsDefault();

protected:
	virtual void InitializeSettings();
	virtual void GoForward();
	virtual void GoBack();

	void OnAnimationStarted_Implementation(const UWidgetAnimation* Animation) override;
	void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	void AddSetting(class UMenuSetting* Setting);

	UFUNCTION(BlueprintCallable, Category = "Video Menu")
		virtual void StoreAllSettings(class UPanelWidget* ParentWidget);

	UPROPERTY(BlueprintReadWrite, Category = "Animations")
		UWidgetAnimation* Animation {};

	UPROPERTY(BlueprintReadWrite, Category = "Menu Base")
		FText MenuTooltipText;

	int32 SelectedMenuIndex;

	UPanelWidget* ParentBox;

	UPROPERTY(BlueprintReadOnly, Category = "Menu Base")
		class AMenuHUD* MenuHUD{};

	TArray<class UMenuSetting*> MenuSettings;

	FTimerHandle ForwardTimerHandle;
	FTimerHandle BackTimerHandle;	
};
