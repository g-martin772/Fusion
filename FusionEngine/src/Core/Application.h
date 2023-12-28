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

        Window* GetPrimaryWindow() { return m_PrimaryWindow; }
    private:
        bool m_Running = true;
        Window* m_PrimaryWindow;
    };
}
