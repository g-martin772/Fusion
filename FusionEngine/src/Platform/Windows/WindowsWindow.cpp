﻿#include "fepch.h"
#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

#include "Core/Application.h"
//#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
#pragma region Callbacks
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        //RenderCommand::ResizeWindow(width, height);
    }

    void window_size_callback(GLFWwindow* window, int width, int height)
    {
        //RenderCommand::ResizeWindow(width, height);
    }
#pragma endregion Callbacks
    
    void WindowsWindow::InitPlatform()
    {
        if (!glfwInit())
            FE_ASSERT(false, "Failed to initialize glfw!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        
        m_Window = glfwCreateWindow(1200, 700, "Fusion Engine", nullptr, nullptr);

        FE_ASSERT(m_Window, "Failed to create glfw window!");

        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
        glfwSetWindowSizeCallback(m_Window, window_size_callback);

        //glfwMakeContextCurrent(m_Window);
    }

    void WindowsWindow::OnUpdatePlatform()
    {
        if(glfwWindowShouldClose(m_Window))
            Application::Get()->Close();

        glfwPollEvents();
        //glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::ShutDownPlatform()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}
