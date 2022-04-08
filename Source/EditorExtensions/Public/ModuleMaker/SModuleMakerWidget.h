// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class EDITOREXTENSIONS_API SModuleMakerWidget : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS( SModuleMakerWidget ) {}

	SLATE_ARGUMENT(FVector2D, WindowSize)
	
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
private:

	EVisibility GetErrorLabelVisibility() const;

	FText GetErrorLabelText() const;

	EVisibility GetChooseFolderVisibility() const;

	EVisibility GetCreateButtonStatus() const;

	FText GetModuleName() const;

	FText GetModulePath() const;

	FText GetHeaderFilePath() const;

	FText GetSourceFilePath() const;

	FText GetConfigFilePath() const;

	void UpdateSourceFilePaths();

	void OnModuleNameChanged(const FText& InputText);

	void OnModuleNameCommitted(const FText& InputText, ETextCommit::Type CommitType);

	void HandleFolderChosen(FString& FolderName);

	FReply HandleChooseFolderButtonClicked();
	
	FReply HandleCreateButtonClicked();

	FReply HandleCancelButtonClicked();

private:
	
	FString NewModuleName;

	FString NewModulePath;

	FString HeaderFile;

	FString SourceFile;

	FString ConfigFile;
	
	bool bLastNameValidityCheckPassed = false;

	bool bLastPathValidityCheckPassed = false;
	
	TSharedPtr<SEditableTextBox> NameEditBox;
	
	FString ErrorString;
};
