// Copyright Overthrone Studios 2019

#include "Log.h"
#include "Engine/Engine.h"
#include "Debug.h"
#include "Math/Vector.h"

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

void ULog::Fatal(const FString& LogMessage)
{
	UE_LOG(LogTemp, Fatal, TEXT("%s"), *LogMessage)
}

void ULog::Error(const FString& LogMessage, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, LogMessage);
	else
		UE_LOG(LogTemp, Error, TEXT("%s"), *LogMessage)
}

void ULog::Warning(const FString& LogMessage, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, LogMessage);
	else
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage)
}

void ULog::Info(const FString& LogMessage, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, LogMessage);
	else
		UE_LOG(LogTemp, Display, TEXT("%s"), *LogMessage)
}

void ULog::Hello(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Hello"));
	else
		UE_LOG(LogTemp, Warning, TEXT("Hello"))
}

void ULog::Yes(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Yes"));
	else
		UE_LOG(LogTemp, Warning, TEXT("Yes"))
}

void ULog::Yes(const FString& Prefix, const bool bLogInViewport)
{
	const FString LogMessage = Prefix + ": Yes";

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, LogMessage);
	else
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage)
}

void ULog::No(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("No"));
	else
		UE_LOG(LogTemp, Warning, TEXT("No"))
}

void ULog::No(const FString& Prefix, const bool bLogInViewport)
{
	const FString LogMessage = Prefix + ": No";

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, LogMessage);
	else
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage)
}

void ULog::Number(const int8 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%i"), Number)
}

void ULog::Number(const int16 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const int32 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const int64 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const float Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::SanitizeFloat(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%f"), Number)
}

void ULog::Number(const double Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::SanitizeFloat(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%f"), Number)
}

void ULog::Number(const long Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%d"), Number)
}

void ULog::Number(const long double Number, const bool bLogInViewport, const float TimeToDisplay)
{
	const FString LongDoubleString = FString::SanitizeFloat(Number);

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, LongDoubleString);
	else
		UE_LOG(LogTemp, Display, TEXT("%s"), *LongDoubleString)
}

void ULog::Bool(const bool bBoolToTest, const bool bLogInViewport, const float TimeToDisplay)
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

void ULog::Vector(const FVector& Vector, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Vector.ToString());
	else
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Vector.ToString())
}

void ULog::LogInt(const int64 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(Number));
	else
		UE_LOG(LogTemp, Display, TEXT("%d"), Number)
}

void ULog::LogUInt(const uint64 Number, const bool bLogInViewport, const float TimeToDisplay)
{
	const auto NewNumber = Number;

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, FString::FromInt(NewNumber));
	else
		UE_LOG(LogTemp, Display, TEXT("%d"), NewNumber)
}
