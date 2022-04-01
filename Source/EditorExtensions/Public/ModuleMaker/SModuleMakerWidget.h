// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum ENameErrorType : uint8
{
	None,
	Duplicate,
	IllegalCharacters
};

/**
 * 
 */
class EDITOREXTENSIONS_API SModuleMakerWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS( SModuleMakerWidget ) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
private:

	EVisibility GetNameErrorLabelVisibility() const;
	
	FText GetNameErrorLabelText() const;
	
	FText GetModuleNameText() const;

	void OnModuleNameChanged(const FText& InputText);

	void OnModuleNameCommitted(const FText&, ETextCommit::Type);

private:

	FString NewModuleName = "NewModule";
	
	bool bIsModulePathValid = false;

	ENameErrorType NameErrorType = None;
	
	/** The editable text box to enter the current name */
	TSharedPtr<SEditableTextBox> ClassNameEditBox;
	
};
