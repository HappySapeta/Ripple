// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#include "EditorExtensions.h"
#include "FRippleActions.h"
#include "LevelEditor.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FEditorExtensionsModule"

void FEditorExtensionsModule::StartupModule()
{
	FRippleActions::Register();
	
	MainMenuExtender = MakeShareable(new FExtender());
	Extension = MainMenuExtender->AddMenuBarExtension("Help", EExtensionHook::After, FRippleActions::Get().ActionList, FMenuBarExtensionDelegate::CreateStatic(&FEditorExtensionsModule::AddMenuExtension));

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
}

void FEditorExtensionsModule::ShutdownModule()
{
	MainMenuExtender->RemoveExtension(Extension.ToSharedRef());
	Extension.Reset();
	MainMenuExtender.Reset();
}

void FEditorExtensionsModule::AddMenuExtension(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(FText::FromString("Ripple"), FText::FromString("Open the Ripple plugin menu"), FNewMenuDelegate::CreateStatic(&FRippleActions::FillMenu));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorExtensionsModule, EditorExtensions)
