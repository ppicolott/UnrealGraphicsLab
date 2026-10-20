using UnrealBuildTool;

public class GraphicsLabShaders : ModuleRules
{
    public GraphicsLabShaders(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "RenderCore" });
    }
}