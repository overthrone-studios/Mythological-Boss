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
			UE_LOG(LogTemp, Display, TEXT("%s"), *LogMessage)
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

	case FATAL:
		UE_LOG(LogTemp, Fatal, TEXT("%s"), *LogMessage)
	break;

	default:
		break;
	}
}

void ULog::DebugMessage(const ELogType LogLevel, const FName& LogMessage, const bool bLogInViewport, const float TimeToDisplay)
{
	switch (LogLevel)
	{
	case INFO:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, LogMessage.ToString());
		else
			UE_LOG(LogTemp, Display, TEXT("%s"), *LogMessage.ToString())
	break;

	case SUCCESS:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, LogMessage.ToString());
		else
			UE_LOG(LogSuccess, Log, TEXT("%s"), *LogMessage.ToString())
	break;

	case WARNING:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, LogMessage.ToString());
		else
			UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage.ToString())
	break;

	case ERROR:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, LogMessage.ToString());
		else
			UE_LOG(LogTemp, Error, TEXT("%s"), *LogMessage.ToString())
	break;

	case FATAL:
		UE_LOG(LogTemp, Fatal, TEXT("%s"), *LogMessage.ToString())
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

void ULog::LogYes(const FString& Prefix, const bool bLogInViewport)
{
	const FString LogMessage = Prefix + ": Yes";

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, LogMessage);
	else
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage)
}

void ULog::LogNo(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("No"));
	else
		UE_LOG(LogTemp, Warning, TEXT("No"))
}

void ULog::LogNo(const FString& Prefix, const bool bLogInViewport)
{
	const FString LogMessage = Prefix + ": No";

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, LogMessage);
	else
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage)
}

void ULog::LogNumber(const int8 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const int16 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	//if (bLogInViewport)
	//	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	//else
	//	UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const int32 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	//if (bLogInViewport)
	//	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	//else
	//	UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const int64 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	//if (bLogInViewport)
	//	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	//else
	//	UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const uint8 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	//if (bLogInViewport)
	//	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	//else
	//	UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const uint16 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	//if (bLogInViewport)
	//	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	//else
	//	UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const uint32 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	//if (bLogInViewport)
	//	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	//else
	//	UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const uint64 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);

	//if (bLogInViewport)
	//	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	//else
	//	UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::LogNumber(const float Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::SanitizeFloat(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%f"), Number)
}

void ULog::LogNumber(const double Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::SanitizeFloat(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%f"), Number)
}

void ULog::LogNumber(const long Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%d"), Number)
}

void ULog::LogNumber(const long double Number, const bool bLogInViewport, const float TimeToDisplay)
{
	const FString LongDoubleString = FString::SanitizeFloat(Number);

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, LongDoubleString);
	else
		UE_LOG(LogTemp, Display, TEXT("%s"), *LongDoubleString)
}

void ULog::LogBool(const bool bBoolToTest, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
	{
		if (bBoolToTest)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "True");
		else
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, "False");
	}
	else
	{
		if (bBoolToTest)
			UE_LOG(LogTemp, Display, TEXT("True"))
		else
			UE_LOG(LogTemp, Display, TEXT("False"))
	}
}

void ULog::LogInt(const int64 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%d"), Number)
}
