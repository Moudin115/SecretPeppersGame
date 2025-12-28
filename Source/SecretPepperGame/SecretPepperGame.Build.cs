using UnrealBuildTool;

public class SecretPepperGame : ModuleRules
{
    public SecretPepperGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "GameplayTags"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "GameplayTags"
        });
    }
}
