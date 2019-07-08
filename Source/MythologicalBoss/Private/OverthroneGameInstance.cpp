// Copyright Overthrone Studios 2019

#include "OverthroneGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UOverthroneGameInstance::UOverthroneGameInstance()
{
	MasterSoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/MasterSoundClass.MasterSoundClass'")));
	MusicSoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/MusicSoundClass.MusicSoundClass'")));
	SFXSoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/SFXSoundClass.SFXSoundClass'")));
	UISoundClass = Cast<USoundClass>(StaticLoadObject(USoundClass::StaticClass(), nullptr, TEXT("SoundClass'/Game/Sounds/Classes/UISoundClass.UISoundClass'")));
	MasterMix = Cast<USoundMix>(StaticLoadObject(USoundMix::StaticClass(), nullptr, TEXT("SoundMix'/Game/Sounds/Mixes/MasterSoundMix.MasterSoundMix'")));

	UGameplayStatics::SetBaseSoundMix(this, MasterMix);
}
