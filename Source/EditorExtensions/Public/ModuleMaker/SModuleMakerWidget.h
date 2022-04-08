// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * This the frontend of the Module Maker.
 * It implements minimal behavior logic
 * to handle user input and invoke necessary actions to easily create new modules. 
 */
class EDITOREXTENSIONS_API SModuleMakerWidget : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS( SModuleMakerWidget ) {}
	
	SLATE_END_ARGS()

	// Constructs the Widget.
	void Construct(const FArguments& InArgs);
	
private:

	// Returns the visibility status of the Error Label.
	EVisibility GetErrorLabelVisibility() const;

	// Returns the Error Label.
	FText GetErrorLabelText() const;

	// Returns the visibility of the 'Choose Folder' button.
	EVisibility GetChooseFolderVisibility() const;

	// Returns the visibility of the 'Create Module' button.
	EVisibility GetCreateButtonVisibility() const;

	// Returns the module's chosen name.
	FText GetModuleName() const;

	// Returns the module's chosen path.
	FText GetModulePath() const;

	// Returns a path to the module's header file.
	FText GetHeaderFilePath() const;

	// Returns a path to the module's source file.
	FText GetSourceFilePath() const;

	// Returns a path to the module's build configuration file.
	FText GetConfigFilePath() const;

	/**
	 * @brief Updates the module's file paths. (Useful when module name or path are changed.)
	 * @param ClearPaths If true, the source file paths are reset to empty strings.
	 */
	void UpdateSourceFilePaths(bool ClearPaths = false);

	// Checks the validity of module name and module path. (Useful when module name or path are changed.)
	void UpdateValidityChecks();
	
	/**
	 * @brief Checks if the new module name is valid, and updates source file paths accordingly. 
	 * @param ModuleNameInput The input provided by the editable text box.
	 */
	void OnModuleNameChanged(const FText& ModuleNameInput);
	
	/**
	 * @brief After a name is committed, a validity check is performed and the editable text box is filled with the new name.
	 * @param ModuleNameInput The input provided by the editable text box.
	 * @param CommitType The way the commit was invoked.
	 */
	void OnModuleNameCommitted(const FText& ModuleNameInput, ETextCommit::Type CommitType);
	
	/**
	 * @brief Folder name is validated, and module path, and source file paths are updated.
	 * @param FolderName The directory that was chosen with the OS's file browser.
	 */
	void HandleFolderChosen(FString& FolderName);

	/**
	 * Launches the OS's file browser that enables the user to choose a directory.
	 * Once a folder/directory is chosen, HandleFolderChosen is invoked.
	 */
	FReply HandleChooseFolderButtonClicked();

	/**
	 * The final checks on module path and module name are performed.
	 * FModuleMaker::CreateNewModule is invoked to generate source files for the new module.
	 * Finally, a dialog is displayed, showing the result of the file generation process.
	 */
	FReply HandleCreateButtonClicked();

	// Destroys the ModuleMaker's window.
	FReply HandleCancelButtonClicked();

private:

	// Contains the new module's name.
	FString NewModuleName;

	// Contains the path to the new module's directory.
	FString NewModulePath;

	// Path to the module's header file.
	FString HeaderFile;

	// Path to the module's source file.
	FString SourceFile;

	// Path to the module's build configuration file.
	FString ConfigFile;

	// Stores the state of the previous name validity check.
	bool bLastNameValidityCheckPassed = false;

	// Stores the state of the previous path validity check.
	bool bLastPathValidityCheckPassed = false;

	// Holds a pointer to the editable text box for the module name entry. 
	TSharedPtr<SEditableTextBox> NameEditBox;

	// Contains the error message generated by various validity checks. 
	FString ErrorString;
};
