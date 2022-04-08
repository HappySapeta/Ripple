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

	static bool IsModuleSourcePathValid(const FString& PathString, const FString& ModuleName, FString& OutFailReason);

	// Creates module files with custom build configuration.
	static bool CreateNewModule(const FString& ModuleName, const FString& ModulePath, FString& OutFailReason);
private:
	static bool CreateModuleSourceFile(const FString& ModuleName, const FString& Template, const FString& TargetFile,
								FString& OutFailReason);
};
