// Copyright Overthrone Studios 2019

#include "Log.h"
#include "Engine/Engine.h"
#include "Debug.h"

void ULog::ObjectValidity(UObject* ObjectRef, const bool bLogInViewport)
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

void ULog::DebugMessage(const ELogType LogLevel, const FString& LogMessage, const bool bLogInViewport, const float TimeToDisplay)
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

void ULog::LogHello(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Hello"));
	else
		UE_LOG(LogTemp, Warning, TEXT("Hello"))
}

void ULog::LogYes(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Yes"));
	else
		UE_LOG(LogTemp, Warning, TEXT("Yes"))
}

void ULog::LogNo(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("No"));
	else
		UE_LOG(LogTemp, Warning, TEXT("No"))
}
