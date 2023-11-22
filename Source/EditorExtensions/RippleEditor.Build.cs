// Copyright [PUBLICATION_YEAR] [MYCOMPANY], Inc. All Rights Reserved.

using UnrealBuildTool;

public class RippleEditor: ModuleRules
{
	public RippleEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
				"Slate",
				"Ripple", 
				"LevelEditor"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange
		(
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
