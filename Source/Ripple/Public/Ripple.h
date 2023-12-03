// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FRippleModule : public FDefaultGameModuleImpl
{
public:
	
	virtual bool SupportsDynamicReloading() override;

#if UE_EDITOR
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

private:
	static bool HandleSettingsSaved();

	static void RegisterSettings(const TWeakObjectPtr<>& SettingsObject, const FText& CategoryDescription, const FName& SectionName, const FText&
								 DisplayName, const FText& Description);

	static void UnregisterSettings();
#endif
};
