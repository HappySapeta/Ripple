// Copyright Anupam Sahu. All Rights Reserved.

#include "Ripple.h"

#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "DeferredBatchProcessor/RpDeferredBatchProcessingSystem.h"

#define LOCTEXT_NAMESPACE "FRippleModule"

bool FRippleModule::SupportsDynamicReloading()
{
	return true;
}

void FRippleModule::StartupModule()
{
	RegisterSettings
	(
		GetMutableDefault<URpDeferredBatchProcessingSystem>(),
		LOCTEXT("RuntimeWDCategoryDescription", "Configure Batch Processing Settings"), 
		"BatchProcessing",
		LOCTEXT("RuntimeGeneralSettingsName", "Batch Processing Settings"),
		LOCTEXT("RuntimeGeneralSettingsDescription", "Configure Batch Processing Settings")
	);
}

void FRippleModule::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

bool FRippleModule::HandleSettingsSaved()
{
	GetMutableDefault<URpDeferredBatchProcessingSystem>()->SaveConfig();
	
	return true;
}

void FRippleModule::RegisterSettings(
	const TWeakObjectPtr<>& SettingsObject,
	const FText& CategoryDescription,
	const FName& SectionName,
	const FText& DisplayName,
	const FText& Description)
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		const ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory
		(
			"Ripple",
			LOCTEXT("RuntimeWDCategoryName", "Ripple"),
			CategoryDescription
		);

		const ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings
		(
			"Project",
			"Ripple",
			SectionName,
			DisplayName,
			Description,
			SettingsObject
		);

		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindStatic(&FRippleModule::HandleSettingsSaved);
		}
	}
}

void FRippleModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "RippleSettings", "BatchProcessing");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRippleModule, Ripple)