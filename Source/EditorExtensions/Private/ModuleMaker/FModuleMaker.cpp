// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/FModuleMaker.h"

#include "GameProjectUtils.h"
#include "LoggingMacros.h"
#include "Interfaces/IMainFrameModule.h"
#include "ModuleMaker/SModuleMakerWidget.h"

#define MAX_MODULE_NAME_LENGTH 32

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

bool FModuleMaker::IsModuleNameValid(const FString& NameString, FString& OutFailReason) 
{
	OutFailReason.Empty();
	
	if(NameString.IsEmpty())
	{
		OutFailReason = "You must specify a module name.";
		return false;
	}

	if(NameString.Contains(" "))
	{
		OutFailReason = "Your module name may not contain a space.";
		return false;
	}

	if(!FChar::IsAlpha(NameString[0]))
	{
		OutFailReason = "Your module name must begin with an alphabetic character.";
		return false;
	}

	if(NameString.Len() > MAX_MODULE_NAME_LENGTH)
	{
		TArray <FStringFormatArg> Args;
		Args.Add(MAX_MODULE_NAME_LENGTH);
		
		OutFailReason = FString::Format(TEXT("The module name must be no longer than {0} characters."), Args);

		return false;
	}

	FString IllegalNameCharacters;
	if ( !GameProjectUtils::NameContainsOnlyLegalCharacters(NameString, IllegalNameCharacters) )
	{
		TArray<FStringFormatArg> Args;
		Args.Add(IllegalNameCharacters);

		OutFailReason = FString::Format(TEXT("The class name may not contain the following characters: '{0}'"), Args);

		return false;
	}
	
	return true;
}

bool FModuleMaker::IsModuleSourcePathValid(const FString& PathString, const FString& ModuleName, FString& OutFailReason)
{
	OutFailReason.Empty();

	if(ModuleName.IsEmpty())
	{
		OutFailReason = "Unable to create a module without a name.";
		return false;
	}
	
	const FString& AbsolutePath = FPaths::ConvertRelativePathToFull(PathString) / "";
	const FString& ModulePath = AbsolutePath + ModuleName / "";

	const FString& HeaderPath = ModulePath / "public" / ModuleName + ".h";
	const FString& SourcePath = ModulePath / "private" / ModuleName + ".cpp";
	const FString& ConfigPath = ModulePath / ModuleName + ".build.cs";

	CLOG(Warning, "%s \n %s \n %s", *HeaderPath, *SourcePath, *ConfigPath);
		
	const bool bSourceFilesAlreadyExist = FPaths::FileExists(HeaderPath) || FPaths::FileExists(SourcePath) || FPaths::FileExists(ConfigPath);

	if(bSourceFilesAlreadyExist)
	{
		OutFailReason = "The chosen folder already contains module files.";
		return false;
	}

	return true;
}

bool FModuleMaker::CreateNewModule(const FString& ModuleName, const FString& OutputPath, FString& OutFailReason)
{
	return false;
}
