// Copyright Overthrone Studios 2019

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Log.generated.h"

// Get the current class name and function name where this is called
#define CUR_CLASS_FUNC (FString(__FUNCTION__))

// Get the current class name and function name where this is called
#define CUR_CLASS_FUNC_WITH_LINE (CUR_CLASS_FUNC + ": " + CUR_LINE)

// Get the current class where this is called
#define CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

// Get the current function name where this is called!
#define CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
  
// Get the current line number in the code where this is called
#define CUR_LINE ("Line " + FString::FromInt(__LINE__))

// Get the current function name and the line number in the code where this is called
#define CUR_FUNC_WITH_LINE  (CUR_FUNC + ": " + CUR_LINE)

// Get the current class and line number where this is called
#define CUR_CLASS_WITH_LINE (CUR_CLASS + "(" + CUR_LINE + ")")
  
// Get the current function signature where this is called
#define CUR_FUNC_SIG (FString(__FUNCSIG__))

UENUM()
enum ELogType
{
	INFO,
	SUCCESS,
	WARNING,
	ERROR,
	FATAL
};

/**
 * A library of log utility functions
 */
UCLASS()
class DEBUG_API ULog final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Log whether the object is valid or not
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void ObjectValidity(UObject* ObjectRef, bool bLogInViewport = false);

	// Log a debug message to the console or viewport (FString version)
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void DebugMessage(ELogType LogLevel, const FString& LogMessage, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a debug message to the console or viewport (FName version)
	static void DebugMessage(ELogType LogLevel, const FName& LogMessage, bool bLogInViewport = false, float TimeToDisplay = 5.0f);	

	// Log a fatal error message to the console and crash
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void Fatal(const FString& LogMessage);

	// Log an error message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void Error(const FString& LogMessage, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a warning message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void Warning(const FString& LogMessage, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log an information message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void Info(const FString& LogMessage, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a hello message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void Hello(bool bLogInViewport = false);
	
	// Log a yes message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void LogYes(bool bLogInViewport = false);

	// Log a no message to the console or viewport
	UFUNCTION(BlueprintCallable, Category = "Debug")
		static void LogNo(bool bLogInViewport = false);

	// Log a yes message to the console or viewport (with an optional prefix string)
	static void LogYes(const FString& Prefix, bool bLogInViewport = false);

	// Log a no message to the console or viewport (with an optional prefix string)
	static void LogNo(const FString& Prefix, bool bLogInViewport = false);

	// Log a number to the console or viewport (int8 version)
	static void Number(int8 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int16 version)
	static void Number(int16 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int32 version)
	static void Number(int32 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int64 version)
	static void Number(int64 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int8 version)
	static void Number(uint8 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int16 version)
	static void Number(uint16 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int32 version)
	static void Number(uint32 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (int64 version)
	static void Number(uint64 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (float version)
	static void Number(float Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (double version)
	static void Number(double Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (long version)
	static void Number(long Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log a number to the console or viewport (long double version)
	static void Number(long double Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

	// Log the given bool value to the console or viewport
	static void Bool(bool bBoolToTest, bool bLogInViewport = false, float TimeToDisplay = 5.0f);

private:
	static void LogInt(int64 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);
	static void LogUInt(uint64 Number, bool bLogInViewport = false, float TimeToDisplay = 5.0f);
};
