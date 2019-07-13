// Copyright Ali El Saleh 2019

#pragma once

#include "Widgets/Settings/AudioSetting.h"
#include "MuteAllSetting.generated.h"

/**
 * Toggles all volumes to mute/un-mute
 */
UCLASS()
class MENUSYSTEM_API UMuteAllSetting final : public UAudioSetting
{
	GENERATED_BODY()
	
public:
	void Init() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Audio")
		void ToggleMute(bool bShouldMute);

	UPROPERTY(config, BlueprintReadWrite, Category = "Audio")
		bool bIsMuted;
	
private:
	class UCheckBox* MuteToggle;
};
