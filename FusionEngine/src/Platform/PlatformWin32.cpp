#include "fepch.h"

#include "Core/Events/Event.h"


#ifdef FE_WINDOWS

#include <windows.h>
#include <windowsx.h>

#include <glm/glm.hpp>

#include "Platform.h"

namespace FusionEngine
{
    static const char* g_windowClassName = "FusionEngineWindowClass";
    static double g_clockFrequency;
    static LARGE_INTEGER g_startTime;

    LRESULT CALLBACK Win32ProcessMessage(HWND window, uint32_t msg, WPARAM wParam, LPARAM lParam);
    
    PlatformState Platform::State;
    
    struct Win32State
    {
        HINSTANCE Instance;
    };

    Result<PlatformState, PlatformError> Platform::Init()
    {
        Win32State* state = new Win32State();

        state->Instance = GetModuleHandle(nullptr);
        
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        g_clockFrequency = 1.0 / static_cast<double>(frequency.QuadPart);
        QueryPerformanceCounter(&g_startTime);

        const HICON icon = LoadIcon(state->Instance, IDI_APPLICATION);
        WNDCLASSA wc; // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa
        wc.style = CS_DBLCLKS;
        wc.lpfnWndProc = Win32ProcessMessage;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = state->Instance;
        wc.hIcon = icon;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = nullptr; // Transparent
        wc.lpszClassName = g_windowClassName;
        wc.lpszMenuName = "idk";

        if (!RegisterClassA(&wc))
        {
            FE_ASSERT(false, "Window class resgistration failed");
            return Err(PlatformError::RegisterWindowClassFailed);
        }
        
        return Ok(PlatformState{state});
    }

    void Platform::Update()
    {
        MSG message;
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
    
    void Platform::ShutDown()
    {
        delete static_cast<Win32State*>(State.InternalState);
    }

    Result<WindowHandle, PlatformError> Platform::CreateNativeWindow()
    {
        const Win32State* state = static_cast<Win32State*>(State.InternalState);
        
        constexpr int32_t client_x = 100;
        constexpr int32_t client_y = 100;
        constexpr int32_t client_width = 1200;
        constexpr int32_t client_height = 800;

        int32_t window_x = client_x;
        int32_t window_y = client_y;
        int32_t window_width = client_width;
        int32_t window_height = client_height;

        constexpr uint32_t window_style =
            WS_OVERLAPPED
            | WS_SYSMENU
            | WS_CAPTION
            | WS_MAXIMIZEBOX
            | WS_MINIMIZEBOX
            | WS_THICKFRAME;
        constexpr uint32_t window_ex_style = WS_EX_APPWINDOW;
        
        RECT border_rect = {0, 0, 0, 0};
        AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

        window_x += border_rect.left;
        window_y += border_rect.top;

        window_width += border_rect.right - border_rect.left;
        window_height += border_rect.bottom - border_rect.top;

        const HWND handle = CreateWindowExA(window_ex_style, g_windowClassName, "Fusion Engine",
                                            window_style, window_x, window_y, window_width, window_height,
                                            nullptr, nullptr, state->Instance, nullptr);

        if (handle == nullptr) {
            FE_ASSERT(false, "Window creation failed");
            return Err(PlatformError::WindowCreationFailed);
        }

        ShowWindow(handle, SW_SHOW);

        return Ok(WindowHandle{handle});
    }
    
    void Platform::DestroyNativeWindow(const WindowHandle handle)
    {
        DestroyWindow(static_cast<HWND>(handle.Handle));
    }

    double Platform::GetAbsTime()
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return g_clockFrequency * static_cast<double>(now.QuadPart);
    }

    void Platform::SleepM(const double ms)
    {
        Sleep(static_cast<DWORD>(ms));
    }

    LRESULT CALLBACK Win32ProcessMessage(HWND window, uint32_t msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            case WM_ERASEBKGND:
                return 1;
        case WM_CLOSE:
                Event::Raise(EventContext(SystemEvent::WindowClose, window));
                return 0;
            case WM_SIZE:
                {
                    RECT r;
                    GetClientRect(window, &r);
                    EventContext e(SystemEvent::WindowResize, window);
                    e.Data.uvec2[0] = glm::uvec2(r.right - r.left, r.bottom - r.top);
                    Event::Raise(e);
                } break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP:
                {
                    
                } break;
            case WM_MOUSEMOVE:
                {
                    glm::uvec2 pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                } break;
            case WM_MOUSEWHEEL:
                {
                    int32_t delta_z = GET_WHEEL_DELTA_WPARAM(wParam);
                    if (delta_z != 0)
                    {
                        delta_z = delta_z < 0 ? -1 : 1;
                    }
                } break;
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
                {
                    bool pressed = msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN;
                } break;
            default: break;
        }

        return DefWindowProcA(window, msg, wParam, lParam);
    }
}

void* operator new(const size_t size)
    {
        return malloc(size);
    }
 
void operator delete(void* p)
{
    free(p);
}

#endif
