// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/FModuleMaker.h"

#include "GameProjectUtils.h"
#include "Interfaces/IMainFrameModule.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
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
		OutFailReason.Append("You must specify a module name.");
		return false;
	}

	if(NameString.Contains(" "))
	{
		OutFailReason.Append("Your module name may not contain a space.");
		return false;
	}

	if(!FChar::IsAlpha(NameString[0]))
	{
		OutFailReason.Append("Your module name must begin with an alphabetic character.");
		return false;
	}

	if(NameString.Len() > MAX_MODULE_NAME_LENGTH)
	{
		TArray <FStringFormatArg> Args;
		Args.Add(MAX_MODULE_NAME_LENGTH);
		
		OutFailReason.Append(FString::Format(TEXT("The module name must be no longer than {0} characters."), Args));

		return false;
	}

	FString IllegalNameCharacters;
	if ( !GameProjectUtils::NameContainsOnlyLegalCharacters(NameString, IllegalNameCharacters) )
	{
		TArray<FStringFormatArg> Args;
		Args.Add(IllegalNameCharacters);

		OutFailReason.Append(FString::Format(TEXT("The class name may not contain the following characters: '{0}'"), Args));

		return false;
	}
	
	return true;
}

bool FModuleMaker::IsModuleSourcePathValid(const FString& PathString, const FString& ModuleName, FString& OutFailReason)
{
	OutFailReason.Empty();

	if(ModuleName.IsEmpty())
	{
		OutFailReason.Append("Unable to create a module without a name.");
		return false;
	}
	
	const FString& AbsolutePath = FPaths::ConvertRelativePathToFull(PathString) / "";
	const FString& ModulePath = AbsolutePath + ModuleName / "";

	const FString& HeaderPath = ModulePath / "public" / ModuleName + ".h";
	const FString& SourcePath = ModulePath / "private" / ModuleName + ".cpp";
	const FString& ConfigPath = ModulePath / ModuleName + ".build.cs";
	
	const bool bSourceFilesAlreadyExist = FPaths::FileExists(HeaderPath) || FPaths::FileExists(SourcePath) || FPaths::FileExists(ConfigPath);

	if(bSourceFilesAlreadyExist)
	{
		OutFailReason.Append("Module source files already exist in the chosen folder.");
		return false;
	}

	return true;
}

bool FModuleMaker::CreateNewModule(const FString& ModuleName, const FString& ModulePath, FString& OutFailReason)
{
	OutFailReason.Empty();

	if(!FPaths::DirectoryExists(ModulePath))
	{
		OutFailReason.Append("Module path does not exist.");
		return false;
	}
	
	const FString& HeaderPath = ModulePath / ModuleName / "Public" / ModuleName + ".h";
	const bool HeaderSuccess = CreateModuleSourceFile(ModuleName, "Module.h.template", HeaderPath, OutFailReason);
	
	const FString& SourcePath = ModulePath / ModuleName / "Private" / ModuleName + ".cpp";
	const bool SourceSuccess = CreateModuleSourceFile(ModuleName, "Module.cpp.template", SourcePath, OutFailReason);
	
	const FString& ConfigPath = ModulePath / ModuleName / ModuleName + "build.cs";
	const bool ConfigSuccess = CreateModuleSourceFile(ModuleName, "Module.build.cs.template", ConfigPath, OutFailReason);
	
	return HeaderSuccess && SourceSuccess && ConfigSuccess;
}

bool FModuleMaker::CreateModuleSourceFile(const FString& ModuleName, const FString& Template, const FString& TargetFile, FString& OutFailReason)
{
	const FString& PluginContentDir = IPluginManager::Get().FindPlugin(TEXT("Ripple"))->GetContentDir();
	const FString& TemplateDir = PluginContentDir / "Editor" / "Templates" / ""; 
	
	FString Content;
	const bool bReadTemplateFileSuccess = FFileHelper::LoadFileToString(Content, *(TemplateDir / Template));

	if(!bReadTemplateFileSuccess)
	{
		OutFailReason.Append("Failed to read template : " + Template);
		return false;
	}
	
	Content = Content.Replace(TEXT("[MODULENAME]"), *ModuleName);
	const bool bWriteFileSuccess = FFileHelper::SaveStringToFile(Content, *TargetFile);

	if(!bReadTemplateFileSuccess)
	{
		OutFailReason.Append("Failed to add source : " + TargetFile);
		return false;
	}
	
	return true;
}
