// Copyright Anupam Sahu. All Rights Reserved.

#include "FRpRippleActions.h"
#include "ModuleMaker/FRpModuleMaker.h"

TSharedRef<FUICommandList> FRpRippleActions::ActionList(new FUICommandList());

FRpRippleActions::FRpRippleActions()
	: TCommands<FRpRippleActions>(FName(TEXT("RPL_MainMenu")), FText::FromString("Ripple Main Menu Commands"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FRpRippleActions::RegisterCommands()
{
	#define LOCTEXT_NAMESPACE ""

	UI_COMMAND(CreateModule, "Create a new module", "Use Ripple's automatic module creation feature to quickly create new modules for your project.", EUserInterfaceActionType::Button, FInputChord());
	ActionList->MapAction(CreateModule, FExecuteAction::CreateStatic(&FRippleActionCallbacks::CreateModule), FCanExecuteAction());

	#undef LOCTEXT_NAMESPACE
}

void FRpRippleActions::FillMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("Modules", FText::FromString("Modules"));
	{
		MenuBuilder.AddMenuEntry
		(
			FRpRippleActions::Get().CreateModule, NAME_None,
			FText::FromString("Create Module"),
			FText::FromString("Create a new module"),
			FSlateIcon()
		);
	}
}

void FRippleActionCallbacks::CreateModule()
{
	FRpModuleMaker::LaunchModuleCreationDialog();
}
