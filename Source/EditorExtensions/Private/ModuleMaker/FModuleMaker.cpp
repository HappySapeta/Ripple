// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/FModuleMaker.h"
#include "Interfaces/IMainFrameModule.h"
#include "ModuleMaker/SModuleMakerWidget.h"

void FModuleMaker::LaunchModuleCreationDialog()
{
	const IMainFrameModule& MainFrameModule = FModuleManager::GetModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	const FVector2D& ModuleMakerWindowSize = MainFrameModule.GetParentWindow()->GetSizeInScreen() * 0.5f;
	
	const TSharedRef<SWindow> ModuleCreationWindow =
		SNew(SWindow)
		.Title(FText::FromString("Add Module"))
		.ClientSize(ModuleMakerWindowSize)
		.SizingRule(ESizingRule::FixedSize)
		.AutoCenter(EAutoCenter::PrimaryWorkArea)
		.SupportsMinimize(false)
		.SupportsMaximize(false);

	ModuleCreationWindow->SetContent(SNew(SModuleMakerWidget).WindowSize(ModuleMakerWindowSize));
	
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
