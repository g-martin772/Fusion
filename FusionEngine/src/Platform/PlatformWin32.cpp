#include "fepch.h"

#include "Core/Application.h"
#include "Core/EventSystem.h"
#include "Core/Window/InputMap.h"
#include "Core/Window/KeyCodes.h"


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
    KeyCode Win32TranslateKeyCode(const uint32_t key, const uint32_t flags);
    
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
            delete state;
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

        const HWND focusedWindow = GetFocus();
        const LONG_PTR lpUserData = GetWindowLongPtr(focusedWindow, GWLP_USERDATA);
        Window* window = reinterpret_cast<Window*>(lpUserData);
        Application::Get()->m_FocusedWindow = window;
    }
    
    void Platform::ShutDown()
    {
        delete static_cast<Win32State*>(State.InternalState);
    }

    Result<WindowHandle, PlatformError> Platform::CreateNativeWindow(Window* instance)
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

        SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));

        ShowWindow(handle, SW_SHOW);

        return Ok(WindowHandle{handle, MakeUnique<InputMap>(), nullptr});
    }

    void Platform::UpdateNativeWindow(WindowHandle& handle)
    {
        const LONG_PTR lpUserData = GetWindowLongPtr(static_cast<HWND>(handle.Handle), GWLP_USERDATA);
        Window* window = reinterpret_cast<Window*>(lpUserData);
    }

    void Platform::DestroyNativeWindow(WindowHandle& handle)
    {
        DestroyWindow(static_cast<HWND>(handle.Handle));
        handle.Handle = nullptr;
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

    LRESULT CALLBACK Win32ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
    {
        const LONG_PTR lpUserData = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        Window* window = reinterpret_cast<Window*>(lpUserData);

        if (window == nullptr)
            return DefWindowProcA(hwnd, msg, wParam, lParam);
        
        InputMap* im = window->GetPlatformHandle()->InputHandle.get();
        
        switch (msg)
        {
            case WM_ERASEBKGND:
                return 1;
            case WM_CLOSE:
                {
                    EventContext e(Event::WindowClose, window->GetPlatformHandle()->Handle);
                    e.Data.ptr[0] = window->GetPlatformHandle()->Handle;
                    EventSystem::Raise(e);
                } return 0;
            case WM_SIZE:
                {
                    RECT r;
                    GetClientRect(hwnd, &r);
                    EventContext e(Event::WindowResize, window->GetPlatformHandle()->Handle);
                    e.Data.uvec2[0] = glm::uvec2(r.right - r.left, r.bottom - r.top);
                    EventSystem::Raise(e);
                } break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                {
                    KeyCode keycode = Win32TranslateKeyCode(wParam, lParam);
                    im->UpdateKeyState(keycode, im->GetKeyState(keycode) == KeyState::Pressed ? KeyState::Down : KeyState::Pressed);
                    EventContext e(Event::KeyPress, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(keycode);
                    EventSystem::Raise(e);
                } break;
            case WM_KEYUP:
            case WM_SYSKEYUP:
                {
                    const KeyCode keycode = Win32TranslateKeyCode(wParam, lParam);
                    im->UpdateKeyState(keycode, im->GetKeyState(keycode) == KeyState::Released ? KeyState::Up : KeyState::Released);
                    EventContext e(Event::KeyRelease, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(keycode);
                    EventSystem::Raise(e);
                } break;
            case WM_MOUSEMOVE:
                {
                    const glm::uvec2 pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                    im->UpdateMousePosition(pos);
                    EventContext e(Event::MouseMoved, window->GetPlatformHandle()->Handle);
                    e.Data.uvec2[0] = pos;
                    EventSystem::Raise(e);
                } break;
            case WM_MOUSEWHEEL:
                {
                    int32_t delta_z = GET_WHEEL_DELTA_WPARAM(wParam);
                    if (delta_z != 0)
                    {
                        delta_z = delta_z < 0 ? -1 : 1;
                        im->UpdateScrollPosition(delta_z);
                        EventContext e(Event::MouseScrolled, window->GetPlatformHandle()->Handle);
                        e.Data.u32[0] = delta_z;
                        EventSystem::Raise(e);
                    }
                } break;
            case WM_LBUTTONDOWN:
                {
                    im->UpdateButtonState(MouseCode::Left, im->GetButtonState(MouseCode::Left) == KeyState::Pressed ? KeyState::Down : KeyState::Pressed);
                    EventContext e(Event::ButtonPress, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(MouseCode::Left);
                    EventSystem::Raise(e);
                } break;
            case WM_LBUTTONUP:
                {
                    im->UpdateButtonState(MouseCode::Left, im->GetButtonState(MouseCode::Left) == KeyState::Released ? KeyState::Up : KeyState::Released);
                    EventContext e(Event::ButtonRelease, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(MouseCode::Left);
                    EventSystem::Raise(e);
                } break;
            case WM_MBUTTONDOWN:
                {
                    im->UpdateButtonState(MouseCode::Middle, im->GetButtonState(MouseCode::Middle) == KeyState::Pressed ? KeyState::Down : KeyState::Pressed);
                    EventContext e(Event::ButtonPress, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(MouseCode::Middle);
                    EventSystem::Raise(e);
                } break;
            case WM_MBUTTONUP:
                {
                    im->UpdateButtonState(MouseCode::Middle, im->GetButtonState(MouseCode::Middle) == KeyState::Released ? KeyState::Up : KeyState::Released);
                    EventContext e(Event::ButtonRelease, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(MouseCode::Middle);
                    EventSystem::Raise(e);
                } break;
            case WM_RBUTTONDOWN:
                {
                    im->UpdateButtonState(MouseCode::Right, im->GetButtonState(MouseCode::Right) == KeyState::Pressed ? KeyState::Down : KeyState::Pressed);
                    EventContext e(Event::ButtonPress, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(MouseCode::Right);
                    EventSystem::Raise(e);
                } break;
            case WM_RBUTTONUP:
                {
                    im->UpdateButtonState(MouseCode::Right, im->GetButtonState(MouseCode::Right) == KeyState::Released ? KeyState::Up : KeyState::Released);
                    EventContext e(Event::ButtonRelease, window->GetPlatformHandle()->Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(MouseCode::Right);
                    EventSystem::Raise(e);
                } break;
            default: break;
        }

        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }

    InputMap* GetIM()
    {
        Window* window = Application::Get()->GetCurrentWindow();

        if(!window)
            return nullptr;

        WindowHandle* handle = window->GetPlatformHandle();

        if (!handle->Handle || !handle->InputHandle)
            return nullptr;
        
        return handle->InputHandle.get();
    }

    bool Platform::IsKeyDown(KeyCode keycode)
    {
        if (const InputMap* im = GetIM())
            return im->GetKeyState(keycode) == KeyState::Down || im->GetKeyState(keycode) == KeyState::Pressed;
        return false;
    }

    bool Platform::IsButtonDown(MouseCode button)
    {
        if (const InputMap* im = GetIM())
            return im->GetButtonState(button) == KeyState::Down || im->GetButtonState(button) == KeyState::Pressed;
        return false;
    }

    glm::uvec2 Platform::GetMouse()
    {
        if(const InputMap* im = GetIM())
            return im->GetMouse();
        return {0, 0};
    }

    glm::uvec2 Platform::GetMouseDelta()
    {
        if (const InputMap* im = GetIM())
            return im->GetMouseDelta();
        return {0, 0};
    }

    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    KeyCode Win32TranslateKeyCode(const uint32_t key, const uint32_t flags)
    {
        switch (key)
        {
            case VK_ESCAPE: return KeyCode::Escape;
            case VK_TAB: return KeyCode::Tab;
            case VK_CAPITAL: return KeyCode::CapsLock;
            case VK_SHIFT: return (flags & 0x01000000) ? KeyCode::RightShift : KeyCode::LeftShift;
            case VK_CONTROL: return (flags & 0x01000000) ? KeyCode::RightControl : KeyCode::LeftControl;
            case VK_MENU: return (flags & 0x01000000) ? KeyCode::RightAlt : KeyCode::LeftAlt;
            case VK_LWIN: return KeyCode::LeftSuper;
            case VK_RWIN: return KeyCode::RightSuper;
            case VK_APPS: return KeyCode::Menu;
            case VK_OEM_1: return KeyCode::Semicolon;
            case VK_OEM_2: return KeyCode::Slash;
            case VK_OEM_3: return KeyCode::GraveAccent;
            case VK_OEM_4: return KeyCode::LeftBracket;
            case VK_OEM_5: return KeyCode::Backslash;
            case VK_OEM_6: return KeyCode::RightBracket;
            case VK_OEM_7: return KeyCode::Apostrophe;
            case VK_OEM_PLUS: return KeyCode::Equal;
            case VK_OEM_MINUS: return KeyCode::Minus;
            case VK_OEM_COMMA: return KeyCode::Comma;
            case VK_OEM_PERIOD: return KeyCode::Period;
            case VK_RETURN: return KeyCode::Enter;
            case VK_BACK: return KeyCode::Backspace;
            case VK_INSERT: return KeyCode::Insert;
            case VK_DELETE: return KeyCode::Delete;
            case VK_HOME: return KeyCode::Home;
            case VK_END: return KeyCode::End;
            case VK_PRIOR: return KeyCode::PageUp;
            case VK_NEXT: return KeyCode::PageDown;
            case VK_NUMLOCK: return KeyCode::NumLock;
            case VK_SCROLL: return KeyCode::ScrollLock;
            case VK_PAUSE: return KeyCode::Pause;
            case VK_PRINT: return KeyCode::PrintScreen;
            case VK_CLEAR: return KeyCode::Clear;
            case VK_SLEEP: return KeyCode::Sleep;
            case VK_F1: return KeyCode::F1;
            case VK_F2: return KeyCode::F2;
            case VK_F3: return KeyCode::F3;
            case VK_F4: return KeyCode::F4;
            case VK_F5: return KeyCode::F5;
            case VK_F6: return KeyCode::F6;
            case VK_F7: return KeyCode::F7;
            case VK_F8: return KeyCode::F8;
            case VK_F9: return KeyCode::F9;
            case VK_F10: return KeyCode::F10;
            case VK_F11: return KeyCode::F11;
            case VK_F12: return KeyCode::F12;
            case VK_F13: return KeyCode::F13;
            case VK_F14: return KeyCode::F14;
            case VK_F15: return KeyCode::F15;
            case VK_F16: return KeyCode::F16;
            case VK_F17: return KeyCode::F17;
            case VK_F18: return KeyCode::F18;
            case VK_F19: return KeyCode::F19;
            case VK_F20: return KeyCode::F20;
            case VK_F21: return KeyCode::F21;
            case VK_F22: return KeyCode::F22;
            case VK_F23: return KeyCode::F23;
            case VK_F24: return KeyCode::F24;
            case VK_NUMPAD0: return KeyCode::KP_0;
            case VK_NUMPAD1: return KeyCode::KP_1;
            case VK_NUMPAD2: return KeyCode::KP_2;
            case VK_NUMPAD3: return KeyCode::KP_3;
            case VK_NUMPAD4: return KeyCode::KP_4;
            case VK_NUMPAD5: return KeyCode::KP_5;
            case VK_NUMPAD6: return KeyCode::KP_6;
            case VK_NUMPAD7: return KeyCode::KP_7;
            case VK_NUMPAD8: return KeyCode::KP_8;
            case VK_NUMPAD9: return KeyCode::KP_9;
            case VK_DECIMAL: return KeyCode::KP_Decimal;
            case VK_DIVIDE: return KeyCode::KP_Divide;
            case VK_MULTIPLY: return KeyCode::KP_Multiply;
            case VK_SUBTRACT: return KeyCode::KP_Subtract;
            case VK_ADD: return KeyCode::KP_Add;
            case VK_SEPARATOR: return KeyCode::KP_Equal;
            case VK_UP: return KeyCode::Up;
            case VK_DOWN: return KeyCode::Down;
            case VK_LEFT: return KeyCode::Left;
            case VK_RIGHT: return KeyCode::Right;
            case VK_SPACE: return KeyCode::Space;
            case 0x30: return KeyCode::D0;
            case 0x31: return KeyCode::D1;
            case 0x32: return KeyCode::D2;
            case 0x33: return KeyCode::D3;
            case 0x34: return KeyCode::D4;
            case 0x35: return KeyCode::D5;
            case 0x36: return KeyCode::D6;
            case 0x37: return KeyCode::D7;
            case 0x38: return KeyCode::D8;
            case 0x39: return KeyCode::D9;
            case 0x41: return KeyCode::A;
            case 0x42: return KeyCode::B;
            case 0x43: return KeyCode::C;
            case 0x44: return KeyCode::D;
            case 0x45: return KeyCode::E;
            case 0x46: return KeyCode::F;
            case 0x47: return KeyCode::G;
            case 0x48: return KeyCode::H;
            case 0x49: return KeyCode::I;
            case 0x4A: return KeyCode::J;
            case 0x4B: return KeyCode::K;
            case 0x4C: return KeyCode::L;
            case 0x4D: return KeyCode::M;
            case 0x4E: return KeyCode::N;
            case 0x4F: return KeyCode::O;
            case 0x50: return KeyCode::P;
            case 0x51: return KeyCode::Q;
            case 0x52: return KeyCode::R;
            case 0x53: return KeyCode::S;
            case 0x54: return KeyCode::T;
            case 0x55: return KeyCode::U;
            case 0x56: return KeyCode::V;
            case 0x57: return KeyCode::W;
            case 0x58: return KeyCode::X;
            case 0x59: return KeyCode::Y;
            case 0x5A: return KeyCode::Z;
            case VK_MEDIA_STOP: return KeyCode::Stop;
            case VK_MEDIA_PLAY_PAUSE: return KeyCode::PlayPause;
            case VK_MEDIA_PREV_TRACK: return KeyCode::MediaPrev;
            case VK_MEDIA_NEXT_TRACK: return KeyCode::MediaNext;
            case VK_VOLUME_DOWN: return KeyCode::VolumeDown;
            case VK_VOLUME_UP: return KeyCode::VolumeUp;
            case VK_VOLUME_MUTE: return KeyCode::VolumeMute;
            case VK_OEM_102: return KeyCode::Pipe;
            default: return KeyCode::None;
        }
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
