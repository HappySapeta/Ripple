// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RIPPLEEDITOR_API FRippleActions : public TCommands<FRippleActions>
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
};


class FRippleActionCallbacks
{
public :
	
	static void CreateModule();
};