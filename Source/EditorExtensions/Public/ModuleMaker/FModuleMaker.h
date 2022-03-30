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

private:
	// Creates module files with custom build configuration.
	static void CreateNewModule();
};
