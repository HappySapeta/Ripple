// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

/**
 * The Module Maker class provides functionality
 * to validate file names, paths and create new module files.
 */
class RIPPLEEDITOR_API FModuleMaker
{
public:
	// Creates a new window and launches the Module Maker's UI.
	static void LaunchModuleCreationDialog();
	
	/**
	 * @brief Performs multiple checks to validate a name for a module.
	 * @param NameString The module name to be tested.
	 * @param RejectReason The reason the name was rejected.
	 * @return True, if the name is valid, false otherwise.
	 */
	static bool IsModuleNameValid(const FString& NameString, FString& RejectReason);
	
	/**
	 * @brief Performs multiple checks to validate a path for a new module.
	 * @param PathString The module path to be tested.
	 * @param ModuleName The name of the new module.
	 * @param RejectReason The reason the path was rejected. 
	 * @return True, if the path is valid, false otherwise.
	 */
	static bool IsModuleSourcePathValid(const FString& PathString, const FString& ModuleName, FString& RejectReason);

	/**
	 * @brief Creates module files (header and source) with custom build configuration.
	 * @param ModuleName The name of the new module.
	 * @param ModulePath The chosen path of the new module.
	 * @param OutFailReason The reason why the module creation failed.
	 * @return True, if module creation was successful, false otherwise.
	 */
	static bool CreateNewModule(const FString& ModuleName, const FString& ModulePath, FString& OutFailReason);
	
private:

	// TODO: This function is useful for creating new files from templates. It needs to be a bit more generic and moved to a more accessible location so that it can be used for general purposes. 
	/**
	 * @brief Creates a modified copy of a template file.
	 * @param TemplateValue A string to replace placeholders. 
	 * @param TemplateFile The file path to a template.
	 * @param TargetFile Destination of the modified copy.
	 * @param OutFailReason The reason why the file creation failed.
	 * @return True, if file creation was successful, false otherwise.
	 */
	static bool CreateModuleSourceFile(const FString& TemplateValue, const FString& TemplateFile, const FString& TargetFile, FString& OutFailReason);
};
