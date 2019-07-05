// Copyright Ali El Saleh 2019

#include "Debug.h"
#include "Engine.h"
#include "MenuSystem.h"

void UDebug::LogObjectValidity(UObject* ObjectRef, const bool bLogInViewport)
{
	if (bLogInViewport)
	{
		if (ObjectRef)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, ObjectRef->GetName() + " is valid");
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Object is null");
	}
	else
	{
		if (ObjectRef)
			UE_LOG(LogTemp, Log, TEXT("%s is valid"), *ObjectRef->GetName())
		else
			UE_LOG(LogTemp, Error, TEXT("Object is null"))
	}
}

void UDebug::LogDebugMessage(const ELogLevel LogLevel, const FString& LogMessage, const bool bLogInViewport, const float TimeToDisplay)
{
	switch (LogLevel)
	{
	case INFO:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, LogMessage);
		else
			UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage)
			break;

	case SUCCESS:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, LogMessage);
		else
			UE_LOG(LogSuccess, Log, TEXT("%s"), *LogMessage)
			break;

	case WARNING:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, LogMessage);
		else
			UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage)
			break;

	case ERROR:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, LogMessage);
		else
			UE_LOG(LogTemp, Error, TEXT("%s"), *LogMessage)
			break;

	default:
		break;
	}
}
