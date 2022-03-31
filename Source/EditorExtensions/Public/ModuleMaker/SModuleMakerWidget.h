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
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
private:

	EVisibility GetNameErrorLabelVisibility() const;

	void OnModuleNameTextChanged(const FText& Text) const;
	
	void OnModuleNameTextCommitted(const FText& Text, ETextCommit::Type Arg) const;
	
	FText OnGetModuleNameText() const;
	
	FText GetNameErrorLabelText() const;

	FText OnGetModulePathText() const;

	void OnModulePathTextChanged(const FText& Text) const;

	FReply HandleChooseFolderButtonClicked() const;

private:

	/** The editable text box to enter the current name */
	TSharedPtr<SEditableTextBox> ClassNameEditBox;
	
};
