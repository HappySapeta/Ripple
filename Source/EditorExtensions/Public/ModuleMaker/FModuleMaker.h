// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EDITOREXTENSIONS_API FModuleMaker
{
public:
	static void LaunchModuleCreationDialog();

	static bool IsModuleNameValid(const FString& NameString, FString& OutFailReason);

	static bool IsSourcePathValid(const FString& PathString, const FString& ModuleName, FString& OutFailReason);
	
private:
	// Creates module files with custom build configuration.
	static bool CreateNewModule(const FString& ModuleName, const FString& OutputPath, FString& OutFailReason);
};
