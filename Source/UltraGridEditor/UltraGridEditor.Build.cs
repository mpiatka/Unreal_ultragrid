// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UltraGridEditor : ModuleRules
{
	public UltraGridEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
				"Core",
				"CoreUObject",
				"Engine",
				"UltraGridCore",
				"Projects",
				"RenderCore",
				"RHI",
				"Slate",
				"SlateCore",
				"UnrealEd",
			}
			);
	}
}
