// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/SModuleMakerWidget.h"

#include "Widgets/Layout/SGridPanel.h"
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
			SNew(SVerticalBox)
#pragma region Title
			// Title 
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0)
			[
				SNew(STextBlock)
				.TextStyle( FEditorStyle::Get(), "NewClassDialog.PageTitle" )
				.Text( LOCTEXT("ModuleMakerTitle", "Name Your New Module") )
			]

			// Title spacer
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 3, 0, 8)
			[
				SNew(SSeparator)
			]
#pragma endregion 

#pragma region Description
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
					.Text(LOCTEXT("ModuleNameCriterion", "Enter a name for your new module. Module names may only contain alphanumeric characters, and may not contain a space."))
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ModuleCreationNotice", "When you click the \"Create\" button below, a header (.h) file, a source (.cpp) and a build configuration (.cs) file will be made using this name."))
				]
			]
#pragma endregion

#pragma region Error Text
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
#pragma endregion

#pragma region User input box
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("DetailsView.CategoryTop"))
				.BorderBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f ))
				.Padding(FMargin(6.0f, 4.0f, 7.0f, 4.0f))
				[
					SNew(SVerticalBox)
#pragma region Module Name Input
					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0)
					[
						SNew(SGridPanel)
						.FillColumn(1, 1.0f)

						// Name Label
						+ SGridPanel::Slot(0, 0)
						.VAlign(VAlign_Center)
						.Padding(0, 0, 120, 0)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text( LOCTEXT( "ModuleNameLabel", "Name" ) )
						]

						// Name input text box
						+ SGridPanel::Slot(1, 0)
						.VAlign(VAlign_Center)
						.Padding(0.0f, 3.0f)
						[
							SNew(SBox)
							.HeightOverride(26.0f)
							[
								SAssignNew(ClassNameEditBox, SEditableTextBox)
								.Text(this, &SModuleMakerWidget::GetModuleNameText)
								.OnTextChanged(this, &SModuleMakerWidget::OnModuleNameChanged)
								.OnTextCommitted(this, &SModuleMakerWidget::OnModuleNameCommitted)
							]
						]
					]
#pragma endregion

#pragma region Module Path Input
					+SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0)
					[
						SNew(SGridPanel)
						.FillColumn(1, 1.0f)

						// Path Label
						+ SGridPanel::Slot(0, 0)
						.VAlign(VAlign_Center)
						.Padding(0, 0, 120, 0)
						[
							SNew(STextBlock)
							.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
							.Text( LOCTEXT( "ModulePathLabel", "Path" ) )
						]

						// Path input text box
						+ SGridPanel::Slot(1, 0)
						.VAlign(VAlign_Center)
						.Padding(0.0f, 3.0f)
						[
							SNew(SBox)
							.HeightOverride(26.0f)
							[
								SAssignNew(ClassNameEditBox, SEditableTextBox)
								.Text(this, &SModuleMakerWidget::GetModuleNameText)
								.OnTextChanged(this, &SModuleMakerWidget::OnModuleNameChanged)
								.OnTextCommitted(this, &SModuleMakerWidget::OnModuleNameCommitted)
							]
						]
					]
#pragma endregion

				]
			]
#pragma endregion 
		]
	];
}
END_FUNCTION_BUILD_OPTIMIZATION


EVisibility SModuleMakerWidget::GetNameErrorLabelVisibility() const
{
	return NameErrorType == ENameErrorType::None ? EVisibility::Hidden : EVisibility::Visible;
}

FText SModuleMakerWidget::GetNameErrorLabelText() const
{
	switch(NameErrorType)
	{
	case Duplicate :
		return LOCTEXT("DuplicateNameErrorText", "A module with this name already exitsts in the choosen folder.");
	case IllegalCharacters:
		return LOCTEXT("IllegalCharactersNameErrorText", "Module name contains illegal characters");
	default:
		return FText::FromString("");
	}
}

FText SModuleMakerWidget::GetModuleNameText() const
{
	return FText::FromString(NewModuleName);
}

void SModuleMakerWidget::OnModuleNameChanged(const FText& InputText)
{
	
}

void SModuleMakerWidget::OnModuleNameCommitted(const FText&, ETextCommit::Type)
{
	
}

#undef LOCTEXT_NAMESPACE
