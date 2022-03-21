// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorExtensions.h"

#define LOCTEXT_NAMESPACE "FEditorExtensionsModule"

void FEditorExtensionsModule::StartupModule()
{
}

void FEditorExtensionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorExtensionsModule, EditorExtensions)