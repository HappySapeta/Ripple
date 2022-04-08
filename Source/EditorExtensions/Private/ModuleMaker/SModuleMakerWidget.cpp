// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleMaker/SModuleMakerWidget.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/FileHelper.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSeparator.h"

#include "ModuleMaker/FModuleMaker.h"

#define MAX_MODULE_NAME_LENGTH 32

BEGIN_FUNCTION_BUILD_OPTIMIZATION
void SModuleMakerWidget::Construct(const FArguments& InArgs)
{
	constexpr float TextBoxHeightOverride = 24.0f;
	
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
			.Padding(0, 0, 0, 5)
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
						.Text(this, &SModuleMakerWidget::GetErrorLabelText)
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
							.Text(this, &SModuleMakerWidget::GetModuleName)
							.HintText(FText::FromString("Name of your module"))
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
								.Text(this, &SModuleMakerWidget::GetModulePath)
								.HintText(FText::FromString("Location of your module"))
								.IsReadOnly(true)
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
							.Visibility(this, &SModuleMakerWidget::GetChooseFolderVisibility)
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
						.TextStyle(FEditorStyle::Get(), "NewClassDialog.SelectedParentClassLabel")
						.Text(FText::FromString("Header File"))
					]

					// Header File Path
					+SGridPanel::Slot(1, 2)
					.Padding(5, 3, 0, 0)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.VAlign(VAlign_Center)
						.HeightOverride(TextBoxHeightOverride)
						[
							SNew(STextBlock)
							.Text(this, &SModuleMakerWidget::GetHeaderFilePath)
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
					.Padding(5, 3, 0, 0)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.VAlign(VAlign_Center)
						.HeightOverride(TextBoxHeightOverride)
						[
							SNew(STextBlock)
							.Text(this, &SModuleMakerWidget::GetSourceFilePath)
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
					.Padding(5, 3, 0, 0)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.VAlign(VAlign_Center)
						.HeightOverride(TextBoxHeightOverride)
						[
							SNew(STextBlock)
							.Text(this, &SModuleMakerWidget::GetConfigFilePath)
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
					.Visibility(this, &SModuleMakerWidget::GetCreateButtonStatus)
					.ContentPadding(FMargin(25.0f, 5.0f))
					.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
					.TextStyle(FEditorStyle::Get(), "LargeText")
					.ForegroundColor(FEditorStyle::Get().GetSlateColor("WhiteBrush"))
					.Text(FText::FromString("Create Module"))
					.OnClicked(this, &SModuleMakerWidget::HandleCreateButtonClicked)
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
					.OnClicked(this, &SModuleMakerWidget::HandleCancelButtonClicked)
				]
			]
#pragma endregion
		]
	];
}
END_FUNCTION_BUILD_OPTIMIZATION

EVisibility SModuleMakerWidget::GetErrorLabelVisibility() const
{
	return ErrorString.IsEmpty() ? EVisibility::Hidden : EVisibility::Visible;
}

FText SModuleMakerWidget::GetErrorLabelText() const
{
	return FText::FromString(ErrorString);
}

EVisibility SModuleMakerWidget::GetChooseFolderVisibility() const
{
	return bLastNameValidityCheckPassed ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SModuleMakerWidget::GetCreateButtonStatus() const
{
	return bLastNameValidityCheckPassed && bLastPathValidityCheckPassed ? EVisibility::Visible : EVisibility::Hidden ;
}

FText SModuleMakerWidget::GetModuleName() const
{
	return FText::FromString(NewModuleName);
}

FText SModuleMakerWidget::GetModulePath() const
{
	return FText::FromString(NewModulePath);
}

FText SModuleMakerWidget::GetHeaderFilePath() const
{
	return FText::FromString(HeaderFile);
}

FText SModuleMakerWidget::GetSourceFilePath() const
{
	return FText::FromString(SourceFile);
}

FText SModuleMakerWidget::GetConfigFilePath() const
{
	return FText::FromString(ConfigFile);
}

void SModuleMakerWidget::UpdateSourceFilePaths()
{
	HeaderFile = NewModulePath / NewModuleName / "public" / NewModuleName + ".h";
	SourceFile = NewModulePath / NewModuleName / "private" / NewModuleName + ".cpp";
	ConfigFile = NewModulePath / NewModuleName / NewModuleName + ".build.cs";
}

void SModuleMakerWidget::OnModuleNameChanged(const FText& InputText)
{
	bLastNameValidityCheckPassed = FModuleMaker::IsModuleNameValid(InputText.ToString(), ErrorString);
	if(bLastNameValidityCheckPassed)
	{
		NewModuleName = InputText.ToString();
		UpdateSourceFilePaths();
	}
	else
	{
		HeaderFile = "";
		SourceFile = "";
		ConfigFile = "";
	}
}

void SModuleMakerWidget::OnModuleNameCommitted(const FText& InputText, ETextCommit::Type CommitType)
{
	if(CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
	{
		if(NameEditBox.IsValid())
		{
			if(bLastNameValidityCheckPassed)
			{
				NameEditBox->SetText(InputText);
			}
			else
			{
				NameEditBox->SetText(FText::FromString(""));
				ErrorString = "You must specify a module name.";
			}
		}
	}
}

void SModuleMakerWidget::HandleFolderChosen(FString& FolderName)
{
	if(!FolderName.EndsWith(TEXT("/"))) { FolderName += TEXT("/"); }
			
	FString FailReason;
	bLastPathValidityCheckPassed = FModuleMaker::IsModuleSourcePathValid(FolderName, NewModuleName, FailReason);
	if(bLastPathValidityCheckPassed)
	{
		NewModulePath = FolderName;
		UpdateSourceFilePaths();
	}
	else
	{
		if(!ErrorString.IsEmpty()) { ErrorString = " " + FailReason; }
		else { ErrorString = FailReason; }
	}
}

FReply SModuleMakerWidget::HandleChooseFolderButtonClicked()
{
	if(!bLastNameValidityCheckPassed) return FReply::Unhandled();
	
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if(DesktopPlatform != nullptr)
	{
		const TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());
		const void* ParentWindowHandle = (ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		FString FolderName;
		const FString& Title = "Choose a source location";

		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog
		(
			ParentWindowHandle,
			Title,
			FPaths::ProjectDir(),
			FolderName
		);

		if(bFolderSelected)
		{
			HandleFolderChosen(FolderName);
		}
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}

FReply SModuleMakerWidget::HandleCreateButtonClicked()
{
	FString HeaderTemplateContent;
	FFileHelper::LoadFileToString(HeaderTemplateContent, *HeaderFile);

	HeaderTemplateContent = HeaderTemplateContent.Replace(TEXT("[MODULENAME]"), *NewModuleName);

	FString SourceTemplateContent;
	FFileHelper::LoadFileToString(SourceTemplateContent, *SourceFile);

	FString ConfigTemplateContent;
	FFileHelper::LoadFileToString(ConfigTemplateContent, *ConfigFile);
	
	return FReply::Handled();
}

FReply SModuleMakerWidget::HandleCancelButtonClicked()
{
	const TSharedPtr<SWindow> ContainingWindow = FSlateApplication::Get().FindWidgetWindow(AsShared());

	if(ContainingWindow.IsValid())
	{
		ContainingWindow->DestroyWindowImmediately();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
