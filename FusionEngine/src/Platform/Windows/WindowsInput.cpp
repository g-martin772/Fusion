#include "fepch.h"
#include "WindowsInput.h"

#include "Core/Application.h"
#include "GLFW/glfw3.h"

namespace FusionEngine
{
    WindowsInput::WindowsInput()
    {
        m_WindowHandle = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());
    }

    bool WindowsInput::IsKeyPressedImpl(KeyCode keycode)
    {
        return glfwGetKey(m_WindowHandle, static_cast<int>(keycode)) == GLFW_PRESS;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button)
    {
        return glfwGetMouseButton(m_WindowHandle, static_cast<int>(button)) == GLFW_PRESS;
    }

    glm::vec2 WindowsInput::GetMousePositionImpl()
    {
        double x, y;
        glfwGetCursorPos(m_WindowHandle, &x, &y);
        return {x, y};
    }

    float WindowsInput::GetMouseXImpl()
    {
        double x;
        glfwGetCursorPos(m_WindowHandle, &x, nullptr);
        return x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        double y;
        glfwGetCursorPos(m_WindowHandle, nullptr, &y);
        return y;
    }
}
