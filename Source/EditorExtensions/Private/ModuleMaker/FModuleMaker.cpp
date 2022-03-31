// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleMaker/FModuleMaker.h"
#include "Interfaces/IMainFrameModule.h"
#include "ModuleMaker/SModuleMakerWidget.h"

#define MODULE_MAKER_WINDOW_SIZE 940, 500

void FModuleMaker::LaunchModuleCreationDialog()
{
	const TSharedRef<SWindow> ModuleCreationWindow =
		SNew(SWindow)
		.Title(FText::FromString("Module Maker"))
		.ClientSize(FVector2D(MODULE_MAKER_WINDOW_SIZE))
		.SizingRule( ESizingRule::FixedSize )
		.AutoCenter(EAutoCenter::PrimaryWorkArea)
		.SupportsMinimize(false)
		.SupportsMaximize(false);

	ModuleCreationWindow->SetContent(SNew(SModuleMakerWidget));
	
	IMainFrameModule& MainFrameModule = FModuleManager::GetModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(ModuleCreationWindow, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(ModuleCreationWindow);
	}
}

void FModuleMaker::CreateNewModule()
{
	
}
