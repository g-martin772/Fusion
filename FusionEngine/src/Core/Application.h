#pragma once
#include <iostream>
#include <memory>

#include "Base.h"
#include "LayerStack.h"
#include "Time.h"
#include "Window.h"
#include "Renderer/Pipeline.h"

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

        Shared<Window> GetWindow() { return m_MainWindow; }
        Shared<Window> GetTestWindow() { return m_TestWindow; }
    private:
        bool m_Running = true;

        Shared<Time> m_Time;
        Shared<Window> m_MainWindow;
        Shared<Window> m_TestWindow;
    };
}
