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
			UE_LOG(LogObjectValidity, Warning, TEXT("%s is valid"), *ObjectRef->GetName())
		else
			UE_LOG(LogObjectValidity, Error, TEXT("Object is null"))	
	}
}

void ULog::DebugMessage(const ELogType LogSeverity, const FString& Message, const bool bLogInViewport, const float TimeToDisplay)
{
	switch (LogSeverity)
	{
	case INFO:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message);
		else
			UE_LOG(LogMessage, Display, TEXT("%s"), *Message)
	break;

	case SUCCESS:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message);
		else
			UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
	break;

	case WARNING:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message);
		else
			UE_LOG(LogMessage, Warning, TEXT("%s"), *Message)
	break;

	case ERROR:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message);
		else
			UE_LOG(LogMessage, Error, TEXT("%s"), *Message)
	break;

	case FATAL:
		UE_LOG(LogMessage, Fatal, TEXT("%s"), *Message)
	break;

	default:
		break;
	}
}

void ULog::DebugMessage(const ELogType LogSeverity, const FName& Message, const bool bLogInViewport, const float TimeToDisplay)
{
	switch (LogSeverity)
	{
	case INFO:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message.ToString());
		else
			UE_LOG(LogMessage, Display, TEXT("%s"), *Message.ToString())
	break;

	case SUCCESS:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message.ToString());
		else
			UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message.ToString())
	break;

	case WARNING:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message.ToString());
		else
			UE_LOG(LogMessage, Warning, TEXT("%s"), *Message.ToString())
	break;

	case ERROR:
		if (bLogInViewport)
			GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message.ToString());
		else
			UE_LOG(LogMessage, Error, TEXT("%s"), *Message.ToString())
	break;

	case FATAL:
		UE_LOG(LogMessage, Fatal, TEXT("%s"), *Message.ToString())
	break;

	default:
		break;
	}
}

void ULog::Fatal(const FString& Message)
{
	UE_LOG(LogMessage, Fatal, TEXT("%s"), *Message)
}

void ULog::Error(const FString& Message, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, Message);
	else
		UE_LOG(LogMessage, Error, TEXT("%s"), *Message)
}

void ULog::Success(const FString& Message, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Green, Message);
	else
		UE_LOG(LogSuccess, Warning, TEXT("%s"), *Message)
}

void ULog::Warning(const FString& Message, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Yellow, Message);
	else
		UE_LOG(LogMessage, Warning, TEXT("%s"), *Message)
}

void ULog::Info(const FString& Message, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Message);
	else
		UE_LOG(LogMessage, Display, TEXT("%s"), *Message)
}

void ULog::Hello(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Hello"));
	else
		UE_LOG(LogMessage, Warning, TEXT("Hello"))
}

void ULog::Yes(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("Yes"));
	else
		UE_LOG(LogBool, Warning, TEXT("Yes"))
}

void ULog::Yes(const FString& Prefix, const bool bLogInViewport)
{
	const FString Message = Prefix + ": Yes";

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
	else
		UE_LOG(LogBool, Warning, TEXT("%s"), *Message)
}

void ULog::No(const bool bLogInViewport)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString("No"));
	else
		UE_LOG(LogBool, Warning, TEXT("No"))
}

void ULog::No(const FString& Prefix, const bool bLogInViewport)
{
	const FString Message = Prefix + ": No";

	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, Message);
	else
		UE_LOG(LogBool, Warning, TEXT("%s"), *Message)
}

void ULog::Number(const int8 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const int16 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const int32 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const int64 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint8 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint16 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint32 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const uint64 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	LogUInt(Number, Prefix, bLogInViewport, TimeToDisplay);
}

void ULog::Number(const float Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::SanitizeFloat(Number));
	else
		UE_LOG(LogNumber, Warning, TEXT("%s%f"), *Prefix, Number)
}

void ULog::Number(const double Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::SanitizeFloat(Number));
	else
		UE_LOG(LogNumber, Warning, TEXT("%s%f"), *Prefix, Number)
}

void ULog::Number(const long Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number));
	else
		UE_LOG(LogNumber, Warning, TEXT("%s%d"), *Prefix, Number)
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
			UE_LOG(LogBool, Warning, TEXT("True"))
		else
			UE_LOG(LogBool, Warning, TEXT("False"))
	}
}

void ULog::Vector(const FVector& Vector, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Vector.ToString());
	else
		UE_LOG(LogVector, Warning, TEXT("%s%s"), *Prefix, *Vector.ToString())
}

void ULog::Rotator(const FRotator& Rotator, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + Rotator.ToString());
	else
		UE_LOG(LogVector, Warning, TEXT("%s%s"), *Prefix, *Rotator.ToString())
}

void ULog::LogInt(const int64 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number));
	else
		UE_LOG(LogNumber, Warning, TEXT("%s%d"), *Prefix, Number)
}

void ULog::LogUInt(const uint64 Number, const FString& Prefix, const bool bLogInViewport, const float TimeToDisplay)
{
	if (bLogInViewport)
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Cyan, Prefix + FString::FromInt(Number));
	else
		UE_LOG(LogNumber, Warning, TEXT("%s%d"), *Prefix, Number)
}
