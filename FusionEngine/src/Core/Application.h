#pragma once
#include <iostream>
#include <memory>

#include "Core/Base.h"
#include "Core/Window/Window.h"

namespace FusionEngine
{
    FE_API class Application
    {
    public:
        static Application* Get();
        
        Application();
        void Run();
        void Shutdown();

        void Close();

        [[nodiscard]] Shared<Window> GetPrimaryWindow() const { return m_PrimaryWindow; }
        [[nodiscard]] Shared<Window> GetCurrentWindow() const { return m_PrimaryWindow; }
        [[nodiscard]] Shared<Window> GetFocusedWindow() const { return m_PrimaryWindow; }
    private:
        bool m_Running = true;
        Shared<Window> m_PrimaryWindow;
        Shared<Window> m_CurrentWindow;
        Shared<Window> m_FocusedWindow;
        List<Shared<Window>> m_Windows;

        friend class Window;
    };
}
