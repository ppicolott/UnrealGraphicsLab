#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

class FGraphicsLabShadersModule : public IModuleInterface
{
public:
        virtual void StartupModule() override
        {
            const FString ShaderDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
            AddShaderSourceDirectoryMapping(TEXT("/GraphicsLab"), ShaderDir);
        }
};

IMPLEMENT_MODULE(FGraphicsLabShadersModule, GraphicsLabShaders)