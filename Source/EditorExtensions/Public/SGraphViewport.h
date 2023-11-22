// Copyright Anupam Sahu. All Rights Reserved.

#pragma once
#include "Selection.h"

class EDITOREXTENSIONS_API SGraphViewport : public SCompoundWidget
{
	
	SLATE_BEGIN_ARGS( SGraphViewport ) {}
	SLATE_END_ARGS()

	// Constructs the Widget.
	void Construct(const FArguments& InArgs);

	// Set the Info Box to be visible only when a Graph Component is selected.
	EVisibility GetInfoBoxVisibility() const;
	
};

inline EVisibility SGraphViewport::GetInfoBoxVisibility() const
{
	for (FSelectionIterator It(GEditor->GetSelectedComponentIterator()); It; ++It)
	{
		if(static_cast<URpSpatialGraphComponent*>(*It))
		{
			return EVisibility::HitTestInvisible;
		}
	}
	
	return EVisibility::Hidden;
}

inline void SGraphViewport::Construct(const FArguments& InArgs)
{
	constexpr FLinearColor TextColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ChildSlot
	.VAlign(VAlign_Bottom)
	[
		SNew(SVerticalBox)
		.Visibility(this, &SGraphViewport::GetInfoBoxVisibility)
		+SVerticalBox::Slot()
		
		.HAlign(HAlign_Right)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Click to select nodes"))
			.ColorAndOpacity(TextColor)
		]
		
		+SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Ctrl + Click to select second node"))
			.ColorAndOpacity(TextColor)
		]
		
		+SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Ctrl + F to join two nodes"))
			.ColorAndOpacity(TextColor)
		]
		
		+SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Alt + X to disconnect two nodes"))
			.ColorAndOpacity(TextColor)
		]
	];
}
