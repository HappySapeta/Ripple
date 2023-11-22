// Copyright Anupam Sahu. All Rights Reserved.

#include "..\Public\RippleEditor.h"
#include "FRippleActions.h"
#include "LevelEditor.h"
#include "RpGraphVisualizer.h"
#include "RpSpatialGraphComponent.h"
#include "SGraphViewport.h"
#include "SLevelViewport.h"
#include "ToolMenus.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"

#define LOCTEXT_NAMESPACE "FRippleEditorModule"

void FRippleEditorModule::StartupModule()
{
	FRippleActions::Register();
	
	MainMenuExtender = MakeShareable(new FExtender());
	Extension = MainMenuExtender->AddMenuBarExtension("Help", EExtensionHook::After, FRippleActions::Get().ActionList, FMenuBarExtensionDelegate::CreateStatic(&FRippleEditorModule::AddMenuExtension));

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
	LevelEditorModule.OnLevelEditorCreated().AddLambda
	(
		[](TSharedPtr<ILevelEditor> LevelEditor)
		{
			TSharedRef<SGraphViewport> GraphViewport = SNew(SGraphViewport);
			GraphViewport->SetVisibility(EVisibility::HitTestInvisible);
			LevelEditor->GetActiveViewportInterface()->AddOverlayWidget(GraphViewport);
		}
	);
	
	if(GUnrealEd)
	{
		TSharedPtr<FRpGraphVisualizer> Visualizer = MakeShareable(new FRpGraphVisualizer());

		if(Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(URpSpatialGraphComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();
		}
	}
}

void FRippleEditorModule::ShutdownModule()
{
	MainMenuExtender->RemoveExtension(Extension.ToSharedRef());
	Extension.Reset();
	MainMenuExtender.Reset();

	if(GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(URpSpatialGraphComponent::StaticClass()->GetFName());
	}
}

void FRippleEditorModule::AddMenuExtension(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(FText::FromString("Ripple"), FText::FromString("Open the Ripple plugin menu"), FNewMenuDelegate::CreateStatic(&FRippleActions::FillMenu));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRippleEditorModule, EditorExtensions)
