// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EDITOREXTENSIONS_API FRippleActions : public TCommands<FRippleActions>
{
public:

	// Creates a new Command Set.
	FRippleActions();

	// Creates UI commands and maps actions.
	virtual void RegisterCommands() override;

	// Adds sections and entries to the plugin's menu.
	static void FillMenu(FMenuBuilder& MenuBuilder);

	// A list containing all command mappings.
	static TSharedRef<FUICommandList> ActionList;

	// Command to launch the module creation menu.
	TSharedPtr<FUICommandInfo> CreateModule;

	// Command to launch the module deletion menu.
	TSharedPtr<FUICommandInfo> DeleteModule;
};


class FRippleActionCallbacks
{
public :
	
	static void CreateModule();

	static void DeleteModule();
	
};