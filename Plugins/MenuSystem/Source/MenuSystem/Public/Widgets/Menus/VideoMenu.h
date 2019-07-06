// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Menus/MenuBase.h"
#include "VideoMenu.generated.h"

/**
 * A video menu contains all settings that changes the visuals
 */
UCLASS()
class MENUSYSTEM_API UVideoMenu final : public UMenuBase
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Back() override;

protected:
	void GoBack() override;

	UPROPERTY(BlueprintReadWrite, Category = "Video Menu")
		TArray<class UVideoSetting*> VideoSettings;
};
