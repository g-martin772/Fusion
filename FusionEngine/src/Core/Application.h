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

        Shared<Window> GetWindow() { return nullptr; }
        Shared<Window> GetTestWindow() { return nullptr; }
    private:
        bool m_Running = true;
    };
}
