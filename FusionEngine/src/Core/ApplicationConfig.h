#pragma once
#include <filesystem>
#include <glm/vec2.hpp>

namespace FusionEngine
{
    class ApplicationConfig
    {
    public:
        void LoadFromArgs(int argc, char** argv);
        void LoadFromConfigFile(const std::filesystem::path& path);

        static void GenerateDefaultConfig(const std::filesystem::path& path);

        std::string ApplicationName = "Fusion Engine";
        bool CreateMainWindow = true;
        bool UseRendering = true;
        bool UseImGui = true;
        bool UseVSync = false;
        bool UseDebugLayer = true;
        bool UseValidationLayers = true;
        bool UseDebugAllocators = true;
        bool UseDebugMarkers = true;
        bool UseDebugNames = true;

        glm::uvec2 MainWindowSize = { 800, 1200 };
        glm::uvec2 MainWindowPosition = { 200, 200 };
        bool MainWindowFullscreen = false;
        bool MainWindowResizable = true;
        bool MainWindowDecorated = true;
        bool MainWindowFocused = true;
        bool MainWindowMaximized = false;

        std::filesystem::path AssetsDirectory;
        std::filesystem::path ShadersDirectory;
        std::filesystem::path EngineDirectory;
    };
    
}
