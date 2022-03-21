// Copyright Epic Games, Inc. All Rights Reserved.

// Logging Macros
// These Macros wrap over UE_LOG and simplify its usage.
// They also offer more useful logging information such as Class name, Function name, and Line number.

#pragma once

// Name of the class where this is called.
#define CLASS_NAME (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))))

// Name of the function where this is called.
#define FUNCTION_NAME (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

// Current Line number in code where this is called, as a string.
#define LINE_NUM (FString::FromInt(__LINE__))

// A composite string containing Class Name, Function Name and Line Number.
#define CALL_LOCATION_FULL ("[" + CLASS_NAME + "::" + FUNCTION_NAME + ":" + LINE_NUM + "]")

/**
 * @brief Logs a message on the console.
 * @param Verbosity Verbosity level to test against.
 * @param FormattedString A character array or a string literal.
 * @param __VA_ARGS__ Variable number of arguments of variable type that must correspond to the parameter type in FormattedString.
 */
#define CLOG(Verbosity, FormattedString, ...) UE_LOG(LogTemp, Verbosity, TEXT("%s"), *FString::Printf(TEXT(FormattedString), ##__VA_ARGS__))

/**
 * @brief Logs a message on the console with the location (Class Name, Function Name) of the call in the code.
 * @param Verbosity Verbosity level to test against.
 * @param FormattedString A character array or string literal.
 * @param __VA_ARGS__ Variable number of arguments of variable type that must correspond to the parameter type in FormattedString.
 */
#define CLOGV(Verbosity, FormattedString, ...) UE_LOG(LogTemp, Verbosity, TEXT("%s %s"), *CALL_LOCATION_FULL, *FString::Printf(TEXT(FormattedString), ##__VA_ARGS__))

/**
 * @brief Logs an unformatted message on the screen.
 * @param FormattedString A simple string without any parameters.
 * @param Duration The duration of time this message will stay on the screen.
 * @param Color The color of the text.
 */
#define SLOG(FormattedString, Duration, Color, ...) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, *FString::Printf(TEXT(FormattedString), ##__VA_ARGS__))

/**
* @brief Logs a unformatted message on the screen with the location (Class Name, Function Name) of the call in the code.
* @param FormattedString A simple string without any parameters.
* @param Duration The duration of time this message will stay on the screen.
* @param Color The color of the text.
*/
#define SLOGV(FormattedString, Duration, Color, ...) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, (CALL_LOCATION_FULL + " " + FString::Printf(TEXT(FormattedString), ##__VA_ARGS__)))
