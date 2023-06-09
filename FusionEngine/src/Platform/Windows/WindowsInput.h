#pragma once
#include "Core/Input.h"
#include "Core/KeyCodes.h"

extern "C" {
    struct GLFWwindow;
}

namespace FusionEngine
{
    class WindowsInput : public Input
    {
    public:
        WindowsInput();
        
        bool IsKeyPressedImpl(KeyCode keycode) override;
        bool IsMouseButtonPressedImpl(MouseCode button) override;
        glm::vec2 GetMousePositionImpl() override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
        void OnUpdateImpl() override;
        void CaptureMouseImpl(bool capture) override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}
