#pragma once
#include "Core/Window.h"

class GLFWwindow;

namespace FusionEngine
{
    class WindowsWindow : public Window
    {
    public:
        void Init() override;
        void OnUpdate() override;
        void ShutDown() override;
    private:
        GLFWwindow* m_Window{};
    };

}
