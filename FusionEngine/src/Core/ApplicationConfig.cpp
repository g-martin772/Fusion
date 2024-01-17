#include "fepch.h"
#include "ApplicationConfig.h"

#include "IO/File.h"

namespace FusionEngine
{
    void ApplicationConfig::LoadFromArgs(int argc, char** argv)
    {
        for (int i = 1; i < argc; i++)
        {
            std::string arg = argv[i-1];


            const auto split = arg.find('=');
            auto key = arg.substr(0, split);
            auto value = arg.substr(split + 1);

            if (key == "--ApplicationName")
                ApplicationName = value;
            else if (key == "--CreateMainWindow")
                CreateMainWindow = value == "true";
            else if (key == "--UseRendering")
                UseRendering = value == "true";
            else if (key == "--UseImGui")
                UseImGui = value == "true";
            else if (key == "--UseVSync")
                UseVSync = value == "true";
            else if (key == "--UseDebugLayer")
                UseDebugLayer = value == "true";
            else if (key == "--UseValidationLayers")
                UseValidationLayers = value == "true";
            else if (key == "--UseDebugAllocators")
                UseDebugAllocators = value == "true";
            else if (key == "--UseDebugMarkers")
                UseDebugMarkers = value == "true";
            else if (key == "--UseDebugNames")
                UseDebugNames = value == "true";
            else if (key == "--MainWindowSize")
            {
                const auto split1 = value.find(',');
                auto x = value.substr(0, split1);
                auto y = value.substr(split1 + 1);

                MainWindowSize.x = std::stoi(x);
                MainWindowSize.y = std::stoi(y);
            }
            else if (key == "--MainWindowPosition")
            {
                const auto split1 = value.find(',');
                auto x = value.substr(0, split1);
                auto y = value.substr(split1 + 1);

                MainWindowPosition.x = std::stoi(x);
                MainWindowPosition.y = std::stoi(y);
            }
            else if (key == "--MainWindowFullscreen")
                MainWindowFullscreen = value == "true";
            else if (key == "--MainWindowResizable")
                MainWindowResizable = value == "true";
            else if (key == "--MainWindowDecorated")
                MainWindowDecorated = value == "true";
            else if (key == "--MainWindowFocused")
                MainWindowFocused = value == "true";
            else if (key == "--MainWindowMaximized")
                MainWindowMaximized = value == "true";
            else if (key == "--AssetsDirectory")
                AssetsDirectory = value;
            else if (key == "--ShadersDirectory")
                ShadersDirectory = value;
            else
                Log::Warn("Unknown config value: {0}", key);
        }
    }

    void ApplicationConfig::LoadFromConfigFile(const std::filesystem::path& path)
    {
        if (!File::Exists(path)) return;

        const auto content = File::ReadLines(path);

        for (auto line : content)
        {
            if (line.empty()) continue;
            if (line[0] == '#') continue;

            const auto split = line.find('=');
            auto key = line.substr(0, split);
            auto value = line.substr(split + 1);

            if (key == "ApplicationName")
                ApplicationName = value;
            else if (key == "CreateMainWindow")
                CreateMainWindow = value == "true";
            else if (key == "UseRendering")
                UseRendering = value == "true";
            else if (key == "UseImGui")
                UseImGui = value == "true";
            else if (key == "UseVSync")
                UseVSync = value == "true";
            else if (key == "UseDebugLayer")
                UseDebugLayer = value == "true";
            else if (key == "UseValidationLayers")
                UseValidationLayers = value == "true";
            else if (key == "UseDebugAllocators")
                UseDebugAllocators = value == "true";
            else if (key == "UseDebugMarkers")
                UseDebugMarkers = value == "true";
            else if (key == "UseDebugNames")
                UseDebugNames = value == "true";
            else if (key == "MainWindowSize")
            {
                const auto split1 = value.find(',');
                auto x = value.substr(0, split1);
                auto y = value.substr(split1 + 1);

                MainWindowSize.x = std::stoi(x);
                MainWindowSize.y = std::stoi(y);
            }
            else if (key == "MainWindowPosition")
            {
                const auto split1 = value.find(',');
                auto x = value.substr(0, split1);
                auto y = value.substr(split1 + 1);

                MainWindowPosition.x = std::stoi(x);
                MainWindowPosition.y = std::stoi(y);
            }
            else if (key == "MainWindowFullscreen")
                MainWindowFullscreen = value == "true";
            else if (key == "MainWindowResizable")
                MainWindowResizable = value == "true";
            else if (key == "MainWindowDecorated")
                MainWindowDecorated = value == "true";
            else if (key == "MainWindowFocused")
                MainWindowFocused = value == "true";
            else if (key == "MainWindowMaximized")
                MainWindowMaximized = value == "true";
            else if (key == "AssetsDirectory")
                AssetsDirectory = value;
            else if (key == "ShadersDirectory")
                ShadersDirectory = value;
            else if (key == "EngineDirectory")
                EngineDirectory = value;
            else
                Log::Warn("Unknown config value: {0}", key);
        }
    }

    void ApplicationConfig::GenerateDefaultConfig(const std::filesystem::path& path)
    {
        File::WriteLines(path, {
            "# Fusion Engine Config",
            "ApplicationName=Fusion Engine",
            "CreateMainWindow=true",
            "UseRendering=true",
            "UseImGui=true",
            "UseVSync=false",
            "UseDebugLayer=true",
            "UseValidationLayers=true",
            "UseDebugAllocators=true",
            "UseDebugMarkers=true",
            "UseDebugNames=true",
            "MainWindowSize=1200,800",
            "MainWindowPosition=200,200",
            "MainWindowFullscreen=false",
            "MainWindowResizable=true",
            "MainWindowDecorated=true",
            "MainWindowFocused=true",
            "MainWindowMaximized=false",
            "AssetsDirectory=assets/",
            "ShadersDirectory=shaders/",
            "EngineDirectory=engine/"
        });
    }
}
