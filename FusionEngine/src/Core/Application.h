#pragma once
#include <iostream>
#include <memory>

#include "ApplicationConfig.h"
#include "Core/Base.h"
#include "Core/Window/Window.h"

namespace FusionEngine
{
    struct RenderModuleData;
}

namespace FusionEngine
{
    class Application
    {
    public:
        static Application* Get();
        
        Application(ApplicationConfig& config);
        void Run();
        void Shutdown();

        void Close();

        [[nodiscard]] const ApplicationConfig& GetConfig() { return m_Config; }

        [[nodiscard]] Window* GetPrimaryWindow() const { return m_PrimaryWindow.get(); }
        [[nodiscard]] Window* GetCurrentWindow() const { return m_CurrentWindow; }
        [[nodiscard]] Window* GetFocusedWindow() const { return m_FocusedWindow; }
    private:
        bool m_Running = true;
        ApplicationConfig m_Config;
        Unique<Window> m_PrimaryWindow;
        Window* m_CurrentWindow;
        Window* m_FocusedWindow;

        RenderModuleData* m_RenderModule;

        friend class Window;
        friend class Platform;
    };
}
