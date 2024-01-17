#include "fepch.h"
#include "Window.h"

#include "Core/Application.h"
#include "Core/Result.h"
#include "Core/EventSystem.h"
#include "Renderer/RenderBackend.h"

namespace FusionEngine
{
    FE_EVENT_CALLBACK_FN(WindowCloseCallback) {
        Window* window = static_cast<Window*>(instance);

        if (window->GetPlatformHandle()->Handle != context.Sender)
            return false;
            
        if (window == Application::Get()->GetPrimaryWindow())
        {
            Application::Get()->Shutdown();
            return true;
        }

        window->ShutDown();
            
        return true;
    }

    Unique<Window> Window::Create(const std::string& name, glm::uvec2 size, glm::uvec2 pos)
    {
        return MakeUnique<Window>(name, size, pos);
    }


    Window::Window(std::string name, const glm::uvec2 size, glm::uvec2 pos): m_Size(size), m_Pos(pos), m_Name(std::move(name))
    {
        auto result = Platform::CreateNativeWindow(this);
        FE_ASSERT(result.is_ok(), "Window creation failed");
        m_PlatformHandle = result.unwrap();

        Log::Trace("Opened window {0}", m_PlatformHandle.Handle);
        
        EventSystem::Register(Event::WindowClose, WindowCloseCallback, this);
        m_IsActive = true;
    }

    Window::~Window()
    {
        ShutDown();
    }

    void Window::OnUpdate()
    {
        Platform::UpdateNativeWindow(m_PlatformHandle);
        m_LayerStack.Update();
    }

    void Window::ShutDown()
    {
        if (!m_IsActive)
            return;
        
        if (!m_PlatformHandle.Handle)
            return;
        Log::Trace("Closing window {0}", m_PlatformHandle.Handle);
        
        Platform::DestroyNativeWindow(m_PlatformHandle);
        EventSystem::Unregister(Event::WindowClose, WindowCloseCallback, this);
        
        m_IsActive = false;
    }

    void Window::MakeCurrent()
    {
        Application::Get()->m_CurrentWindow = this;
    }

    Window* Window::Current()
    {
        return Application::Get()->GetCurrentWindow();
    }

    void Window::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Window::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Window::PopLayer(Layer* layer)
    {
        m_LayerStack.PopLayer(layer);
    }

    void Window::PopOverlay(Layer* overlay)
    {
        m_LayerStack.PopOverlay(overlay);
    }
}
