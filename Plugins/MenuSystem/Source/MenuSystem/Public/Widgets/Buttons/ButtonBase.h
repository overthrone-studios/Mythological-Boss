// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ButtonBase.generated.h"

UENUM()
enum EButtonType
{
	BTN_NEW_GAME	UMETA(DisplayName = "New game"),
	BTN_CONTINUE	UMETA(DisplayName = "Continue"),
	BTN_OPTIONS		UMETA(DisplayName = "Options"),
	BTN_CREDITS		UMETA(DisplayName = "Credits"),
	BTN_VIDEO		UMETA(DisplayName = "Video"),
	BTN_AUDIO		UMETA(DisplayName = "Audio"),
	BTN_CONTROLS	UMETA(DisplayName = "Controls"),
	BTN_RESET_INPUT	UMETA(DisplayName = "Reset input bindings"),
	BTN_APPLY		UMETA(DisplayName = "Apply"),
	BTN_BACK		UMETA(DisplayName = "Back"),
	BTN_EXIT		UMETA(DisplayName = "Exit"),
	BTN_NONE		UMETA(DisplayName = "None")
};

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API UButtonBase : public UUserWidget
{
	GENERATED_BODY()
	
	
	
	
};
