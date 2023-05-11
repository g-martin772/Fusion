#include "fepch.h"
#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

#include "Core/Application.h"

namespace FusionEngine
{
    void WindowsWindow::Init()
    {
        if (!glfwInit())
            FE_ASSERT(false, "Failed to initialize glfw!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        m_Window = glfwCreateWindow(1200, 700, "Fusion Engine", nullptr, nullptr);

        if(!m_Window)
            FE_ASSERT(false, "Failed to create glfw window!");

        glfwMakeContextCurrent(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        if(glfwWindowShouldClose(m_Window))
            Application::Get()->SetRunning(false);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void WindowsWindow::ShutDown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}
