// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/SModuleMakerWidget.h"
#include "Widgets/Layout/SSeparator.h"

#define LOCTEXT_NAMESPACE "ModuleMaker"

BEGIN_FUNCTION_BUILD_OPTIMIZATION
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
					.Padding(5, 2)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(0.125f)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text(LOCTEXT("ModuleNameLabel", "Name"))
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew( ClassNameEditBox, SEditableTextBox)
							.Text( this, &SModuleMakerWidget::OnGetModuleNameText )
							.OnTextChanged( this, &SModuleMakerWidget::OnModuleNameTextChanged )
							.OnTextCommitted( this, &SModuleMakerWidget::OnModuleNameTextCommitted )
						]
					]

					+ SVerticalBox::Slot()
					.Padding(5, 2)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(0.2f)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text(LOCTEXT("ModulePathLabel", "Path"))
						]

						// Path edit box
						+SHorizontalBox::Slot()
						.Padding(0.0f, 3.0f)
						.VAlign(VAlign_Center)
						[
							SNew(SVerticalBox)
							// Native C++ path
							+ SVerticalBox::Slot()
							.Padding(0)
							.AutoHeight()
							[
								SNew(SBox)
								.Visibility(EVisibility::Visible)
								.HeightOverride(26.0f)
								[
									SNew(SHorizontalBox)
									+SHorizontalBox::Slot()
									.FillWidth(1.0f)
									[
										SNew(SEditableTextBox)
										.Text(this, &SModuleMakerWidget::OnGetModulePathText)
										.OnTextChanged(this, &SModuleMakerWidget::OnModulePathTextChanged)
									]
									
									+SHorizontalBox::Slot()
									.AutoWidth()
									.Padding(6.0f, 1.0f, 0.0f, 0.0f)
									[
										SNew(SButton)
										.VAlign(VAlign_Center)
										.OnClicked(this, &SModuleMakerWidget::HandleChooseFolderButtonClicked)
										.Text( LOCTEXT( "BrowseButtonText", "Choose Folder" ) )
									]
								]
							]
						]
					]

					+ SVerticalBox::Slot()
					.Padding(5, 2)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(0.25f)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text(LOCTEXT("ModuleHeaderLabel", "Header File"))
						]

						+SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text( LOCTEXT("ModuleHeaderFilePath","D:/Work/MyWork/Unreal/Projects/UnrealDevLab/Source/UnrealDevLab/Module/Public/Module.h") )
						]
					]

					+ SVerticalBox::Slot()
					.Padding(5, 2)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(0.25f)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text(LOCTEXT("ModuleSourceLabel", "Source File"))
						]

						+SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.AutoWidth()
						[
							SNew(STextBlock)
							.Text( LOCTEXT("ModuleSourceFilePath","D:/Work/MyWork/Unreal/Projects/UnrealDevLab/Source/UnrealDevLab/Module/Private/Module.cpp") )
						]
					]

					+ SVerticalBox::Slot()
					.Padding(5, 2)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.FillWidth(0.25f)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text(LOCTEXT("ModuleConfigLabel", "Configuration File"))
						]

						+SHorizontalBox::Slot()
						.AutoWidth()
						.HAlign(HAlign_Left)
						[
							SNew(STextBlock)
							.Text( LOCTEXT("ModuleConfigFilePath","D:/Work/MyWork/Unreal/Projects/UnrealDevLab/Source/UnrealDevLab/Module/Module.build.cs") )
						]
					]
				]
			]
		]
	];
}
END_FUNCTION_BUILD_OPTIMIZATION


EVisibility SModuleMakerWidget::GetNameErrorLabelVisibility() const
{
	return EVisibility::Hidden;
}

void SModuleMakerWidget::OnModuleNameTextChanged(const FText& Text) const
{
}

void SModuleMakerWidget::OnModuleNameTextCommitted(const FText& Text, ETextCommit::Type Arg) const
{
}

FText SModuleMakerWidget::OnGetModuleNameText() const
{
	return LOCTEXT("ModuleNameText", "SomeName");
}

FText SModuleMakerWidget::GetNameErrorLabelText() const
{
	return LOCTEXT("ModuleNameError", "Invalid Name");
}

FText SModuleMakerWidget::OnGetModulePathText() const
{
	return LOCTEXT("ModulePathText", "SomePath");
}

void SModuleMakerWidget::OnModulePathTextChanged(const FText& Text) const
{
}

FReply SModuleMakerWidget::HandleChooseFolderButtonClicked() const
{
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
