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

	FText GetModuleNameText() const;
	
	FText GetModulePathText() const;

	// Check if the Module name is valid. 
	bool IsModuleNameValid(const FString& InputString, FString& OutFailReason) const;

	// Check with the OS if this path exists.
	bool IsModulePathValid(const FString& InputText, FString& OutFailReason) const;

	void OnModuleNameChanged(const FText& InputText);

	void OnModuleNameCommitted(const FText& InputText, ETextCommit::Type CommitType);

	FReply HandleChooseFolderButtonClicked();
	
	FReply HandleCreateButtonClicked();

	FReply HandleCancelButtonClicked();

	void UpdateNameValidity();

private:
	
	FText NewModuleName;

	FText NewModulePath;

	TSharedPtr<SEditableTextBox> NameEditBox;
	
	FString ErrorString;
};
