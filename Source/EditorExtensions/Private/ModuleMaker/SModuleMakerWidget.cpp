// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/SModuleMakerWidget.h"

#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"

#include "GameProjectUtils.h"
#include "LoggingMacros.h"

#define MAX_MODULE_NAME_LENGTH 32

BEGIN_FUNCTION_BUILD_OPTIMIZATION
void SModuleMakerWidget::Construct(const FArguments& InArgs)
{
	const float TextBoxHeightOverride = 24.0f;
	
	ChildSlot
	[
		SNew(SBorder)
		.Padding(18)
		.BorderImage(FEditorStyle::GetBrush("Docking.Tab.ContentAreaBrush"))
		[
			SNew(SVerticalBox)


#pragma region Title
			// Title 
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "NewClassDialog.PageTitle")
				.Text(FText::FromString("Name Your New Module"))
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
					.Text(FText::FromString("Enter a name for your new module. Module names may only contain alphanumeric characters, and may not contain a space."))
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 5)
				[
					SNew(STextBlock)
					.Text(FText::FromString("When you click the \"Create\" button below, a header (.h) file, a source (.cpp) and a build configuration (.cs) file will be made using this name."))
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
					.Visibility( this, &SModuleMakerWidget::GetErrorLabelVisibility)
					.BorderImage(FEditorStyle::GetBrush("NewClassDialog.ErrorLabelBorder"))
					.Content()
					[
						SNew(STextBlock)
						.Text( this, &SModuleMakerWidget::GetErrorLabelText)
						.TextStyle(FEditorStyle::Get(), "NewClassDialog.ErrorLabelFont")
					]
				]
			]
#pragma endregion

#pragma region Details View
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("DetailsView.CategoryTop"))
				.BorderBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f ))
				.Padding(FMargin(6.0f, 4.0f, 7.0f, 4.0f))
				[
					SNew(SGridPanel)
					.FillColumn(0, 0.15f)
					.FillColumn(1, 0.85f)

					// Name Label
					+ SGridPanel::Slot(0, 0)
					.Padding(0, 3, 0, 0)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.TextStyle(FEditorStyle::Get(),"NewClassDialog.SelectedParentClassLabel")
						.Text(FText::FromString("Name"))
					]

					// Name Input Text Box
					+SGridPanel::Slot(1, 0)
					.Padding(0, 3, 0, 0)
					[
						SNew(SBox)
						.HeightOverride(TextBoxHeightOverride)
						[
							SAssignNew(NameEditBox, SEditableTextBox)
							.Text(this, &SModuleMakerWidget::GetModuleNameText)
							.HintText( FText::FromString("Name of your module"))
							.OnTextChanged(this, &SModuleMakerWidget::OnModuleNameChanged)
							.OnTextCommitted(this, &SModuleMakerWidget::OnModuleNameCommitted)
						]
					]
					
					// Path Label
					+ SGridPanel::Slot(0, 1)
					.VAlign(VAlign_Center)
					.Padding(0, 3, 0, 0)
					[
						SNew(STextBlock)
						.TextStyle(FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel")
						.Text(FText::FromString("Path"))
					]
					
					// Path input text box
					+ SGridPanel::Slot(1, 1)
					.VAlign(VAlign_Center)
					.Padding(0, 3, 0, 0)
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						[
							SNew(SBox)
							.HeightOverride(TextBoxHeightOverride)
							[
								SNew(SEditableTextBox)
								.Text(this, &SModuleMakerWidget::GetModulePathText)
								.HintText(FText::FromString("Location of your module"))
							]
						]
						
						// Choose Folder Button
						+SHorizontalBox::Slot()
						.Padding(5, 0, 0, 0)
						.AutoWidth()
						[
							SNew(SButton)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Center)
							.OnClicked(this, &SModuleMakerWidget::HandleChooseFolderButtonClicked)
							.Text(FText::FromString("Choose Folder"))
						]
					]

					// Header File Label
					+SGridPanel::Slot(0, 2)
					.VAlign(VAlign_Center)
					.Padding(0, 3, 0, 0)
					[
						SNew(STextBlock)
						.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
						.Text(FText::FromString("Header File"))
					]

					// Header File Path
					+SGridPanel::Slot(1, 2)
					.Padding(0, 3, 0, 0)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.VAlign(VAlign_Center)
						.HeightOverride(TextBoxHeightOverride)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Header path goes here."))
						]
					]

					// Source File Label
					+SGridPanel::Slot(0, 3)
					.VAlign(VAlign_Center)
					.Padding(0, 3, 0, 0)
					[
						SNew(STextBlock)
						.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
						.Text(FText::FromString("Source File"))
					]

					// Source File Path
					+SGridPanel::Slot(1, 3)
					.Padding(0, 3, 0, 0)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.VAlign(VAlign_Center)
						.HeightOverride(TextBoxHeightOverride)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Source path goes here."))
						]
					]

					// Build Configuration File Label
					+SGridPanel::Slot(0, 4)
					.VAlign(VAlign_Center)
					.Padding(0, 3, 0, 0)
					[
						SNew(STextBlock)
						.TextStyle( FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel" )
						.Text(FText::FromString("Build Config File"))
					]

					// Build Configuration File Path
					+SGridPanel::Slot(1, 4)
					.Padding(0, 3, 0, 0)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.VAlign(VAlign_Center)
						.HeightOverride(TextBoxHeightOverride)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Build Configuration path goes here."))
						]
					]
				]
			]
#pragma endregion

#pragma region Menu Buttons
			+SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Right)
			[
				SNew(SHorizontalBox)

				// Create Module Button
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.ContentPadding(FMargin(25.0f, 5.0f))
					.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
					.TextStyle(FEditorStyle::Get(), "LargeText")
					.ForegroundColor(FEditorStyle::Get().GetSlateColor("WhiteBrush"))
					.Text(FText::FromString("Create Module"))
					//.OnClicked(this, &HandleCreateButtonClicked)
				]

				// Cancel Button
				+SHorizontalBox::Slot()
				.Padding(10, 0, 0, 0)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.ContentPadding(FMargin(25.0f, 5.0f))
					.ButtonStyle(FEditorStyle::Get(), "FlatButton.Default")
					.TextStyle(FEditorStyle::Get(), "LargeText")
					.ForegroundColor(FEditorStyle::Get().GetSlateColor("WhiteBrush"))
					.Text(FText::FromString("Cancel"))
					//.OnClicked(this, &HandleCancelButtonClicked)
				]
			]
#pragma endregion
		]
	];
}

EVisibility SModuleMakerWidget::GetErrorLabelVisibility() const
{
	return ErrorString.IsEmpty() ? EVisibility::Hidden : EVisibility::Visible;
}

FText SModuleMakerWidget::GetErrorLabelText() const
{
	return FText::FromString(ErrorString);
}

FText SModuleMakerWidget::GetModuleNameText() const
{
	return NewModuleName;
}

FText SModuleMakerWidget::GetModulePathText() const
{
	return NewModulePath;
}

bool SModuleMakerWidget::IsModuleNameValid(const FString& InputString, FString& OutFailReason) const
{
	OutFailReason.Empty();
	
	if(InputString.IsEmpty())
	{
		OutFailReason = "You must specify a module name.";
		return false;
	}

	if(InputString.Contains(" "))
	{
		OutFailReason = "Your module name may not contain a space.";
		return false;
	}

	if(!FChar::IsAlpha(InputString[0]))
	{
		OutFailReason = "Your module name must begin with an alphabetic character.";
		return false;
	}

	if(InputString.Len() > MAX_MODULE_NAME_LENGTH)
	{
		TArray <FStringFormatArg> Args;
		Args.Add(MAX_MODULE_NAME_LENGTH);
		
		OutFailReason = FString::Format(TEXT("The module name must be no longer than {0} characters."), Args);

		return false;
	}

	FString IllegalNameCharacters;
	if ( !GameProjectUtils::NameContainsOnlyLegalCharacters(InputString, IllegalNameCharacters) )
	{
		TArray<FStringFormatArg> Args;
		Args.Add(IllegalNameCharacters);

		OutFailReason = FString::Format(TEXT("The class name may not contain the following characters: '{IllegalNameCharacters}'"), Args);

		return false;
	}
	
	return true;
}

bool SModuleMakerWidget::IsModulePathValid(const FString& InputText, FString& OutFailReason) const
{
	OutFailReason.Empty();
	return true;
}

void SModuleMakerWidget::OnModuleNameChanged(const FText& InputText)
{
	UpdateNameValidity();
}

void SModuleMakerWidget::OnModuleNameCommitted(const FText& InputText, ETextCommit::Type CommitType)
{
}

FReply SModuleMakerWidget::HandleChooseFolderButtonClicked()
{
	return FReply::Handled();
}

FReply SModuleMakerWidget::HandleCreateButtonClicked()
{
	return FReply::Handled();
}

FReply SModuleMakerWidget::HandleCancelButtonClicked()
{
	return FReply::Handled();
}

void SModuleMakerWidget::UpdateNameValidity()
{
	if(!NameEditBox.IsValid())
	{
		CLOGV(Error, "NameEditBox has expired.");
	}
	
	FString FailReason;
	bool bIsNameValid = IsModuleNameValid(NameEditBox->GetText().ToString(), FailReason); 
}

END_FUNCTION_BUILD_OPTIMIZATION

