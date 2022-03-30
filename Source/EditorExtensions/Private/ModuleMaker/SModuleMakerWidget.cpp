// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleMaker/SModuleMakerWidget.h"

#define LOCTEXT_NAMESPACE "MainMenu"

void SModuleMakerWidget::Construct(const FArguments& InArgs)
{
	const FMargin ContentPadding(500.0f, 300.0f);
	const FText TitleText = FText::FromString("Name Your New Module");
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f ))
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(ContentPadding)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(TitleText)
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE
