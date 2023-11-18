// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

using UnrealBuildTool;

public class EditorExtensions: ModuleRules
{
	public EditorExtensions(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Slate",
				"Ripple"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"SlateCore",
				"UnrealEd", 
				"MainFrame",
				"EditorStyle",
				"ToolMenus",
				"GameProjectGeneration",
				"DesktopPlatform",
				"Projects",
				"ToolWidgets",
				"ComponentVisualizers",
				"InputCore"
				// ... add private dependencies that you statically link with here ...	
			}
			);
	}
}
