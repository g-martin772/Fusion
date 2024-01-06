#pragma once
#include <iostream>
#include <memory>

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
        
        Application();
        void Run();
        void Shutdown();

        void Close();

        [[nodiscard]] Window* GetPrimaryWindow() const { return m_PrimaryWindow; }
        [[nodiscard]] Window* GetCurrentWindow() const { return m_PrimaryWindow; }
        [[nodiscard]] Window* GetFocusedWindow() const { return m_PrimaryWindow; }
    private:
        bool m_Running = true;
        Window* m_PrimaryWindow;
        Window* m_CurrentWindow;
        Window* m_FocusedWindow;

        RenderModuleData* m_RenderModule;

        friend class Window;
        friend class Platform;
    };
}
