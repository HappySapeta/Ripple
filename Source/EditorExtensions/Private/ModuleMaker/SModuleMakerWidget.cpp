// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/SModuleMakerWidget.h"
#include "Widgets/Layout/SSeparator.h"

#define LOCTEXT_NAMESPACE "ModuleMaker"

void SModuleMakerWidget::Construct(const FArguments& InArgs)
{	
	ChildSlot
	[
		SNew(SBorder)
		.Padding(18)
		.BorderImage( FEditorStyle::GetBrush("Docking.Tab.ContentAreaBrush") )
		[
			// Title 
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0)
			[
				SNew(STextBlock)
				.TextStyle( FEditorStyle::Get(), "NewClassDialog.PageTitle" )
				.Text( LOCTEXT("ModuleTitle", "Name Your New Module") )
			]

			// Title spacer
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 2, 0, 8)
			[
				SNew(SSeparator)
			]

			// Description
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 5)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ModuleNameCriteria", "Enter a name for your new module. Module names may only contain alphanumeric characters, and may not contain a space."))
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ModuleCreationNotice", "When you click the \"Create\" button below, a header (.h) file, a source (.cpp) and a build configuration (.cs) file will be made using this name."))
				]
			]

			// Name Error label
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				// Constant height, whether the label is visible or not
				SNew(SBox).HeightOverride(20)
				[
					SNew(SBorder)
					.Visibility( this, &SModuleMakerWidget::GetNameErrorLabelVisibility )
					.BorderImage( FEditorStyle::GetBrush("NewClassDialog.ErrorLabelBorder") )
					.Content()
					[
						SNew(STextBlock)
						.Text( this, &SModuleMakerWidget::GetNameErrorLabelText )
						.TextStyle( FEditorStyle::Get(), "NewClassDialog.ErrorLabelFont" )
					]
				]
			]

			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 5)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("DetailsView.CategoryTop"))
				.BorderBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f ))
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.FillHeight(0.25f)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(0.125f)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text(LOCTEXT("ModuleNameLabel", "Name"))
						]

						+SHorizontalBox::Slot()
						.FillWidth(0.5f)
						[
							SAssignNew( ClassNameEditBox, SEditableTextBox)
							.Text( this, &SModuleMakerWidget::OnGetModuleNameText )
							.OnTextChanged( this, &SModuleMakerWidget::OnModuleNameTextChanged )
							.OnTextCommitted( this, &SModuleMakerWidget::OnModuleNameTextCommitted )
						]
					]
				]
			]
		]
	];
}



EVisibility SModuleMakerWidget::GetNameErrorLabelVisibility() const
{
	return EVisibility::Visible;
}

void SModuleMakerWidget::OnModuleNameTextChanged(const FText& Text) const
{
}

void SModuleMakerWidget::OnModuleNameTextCommitted(const FText& Text, ETextCommit::Type Arg) const
{
}

FText SModuleMakerWidget::OnGetModuleNameText() const
{
	return LOCTEXT("SomeKey", "SomeName");
}

FText SModuleMakerWidget::GetNameErrorLabelText() const
{
	return LOCTEXT("ModuleNameError", "Invalid Name");
}

#undef LOCTEXT_NAMESPACE
