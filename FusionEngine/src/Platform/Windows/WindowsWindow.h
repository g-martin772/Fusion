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
        void InitPlatform() override;
        void OnUpdatePlatform() override;
        void ShutDownPlatform() override;
        void* GetNativeWindow() override { return m_Window; }
    private:
        GLFWwindow* m_Window{};
        std::thread m_EventThread;
    };

}
