#pragma once
#include "Core/Window.h"

extern "C"{
    struct GLFWwindow;
}

namespace FusionEngine
{
    class WindowsWindow : public Window
    {
    public:
        void Init() override;
        void OnUpdate() override;
        void ShutDown() override;
        void* GetNativeWindow() override { return m_Window; }
    private:
        GLFWwindow* m_Window{};
        std::thread m_EventThread;
    };

}
