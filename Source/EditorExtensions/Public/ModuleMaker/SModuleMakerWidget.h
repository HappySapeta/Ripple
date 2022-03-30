// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

/**
 * 
 */
class EDITOREXTENSIONS_API SModuleMakerWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS( SModuleMakerWidget ) {}
	
	// A reference to the parent window
	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwningHUD)

	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override { return true; }
		
	// The slate window that created this widget.
	TSharedPtr<SWindow> ParentWindow;
};
