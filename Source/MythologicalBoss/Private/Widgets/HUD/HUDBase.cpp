// Copyright Overthrone Studios 2019

#include "Widgets/HUD/HUDBase.h"
#include "OverthroneHUD.h"
#include "Kismet/GameplayStatics.h"

void UHUDBase::Init()
{
	OverthroneHUD = Cast<AOverthroneHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
}
