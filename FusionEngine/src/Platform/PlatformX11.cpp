#include "fepch.h"

#ifdef FE_X11

#include "Core/Application.h"
#include "Core/Events/EventSystem.h"
#include "Core/Window/InputMap.h"
#include "Core/Window/KeyCodes.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

#include <glm/glm.hpp>

#include "Platform.h"

#undef None

namespace FusionEngine {
    KeyCode X11TranslateKeyCode(const uint32_t key);
    PlatformState Platform::State;
}


struct X11State
{
    Display* display;
    int screen;
};

FusionEngine::Result<FusionEngine::PlatformState, FusionEngine::PlatformError> FusionEngine::Platform::Init()
{
    X11State* state = new X11State();

    state->display = XOpenDisplay(nullptr);
    if (state->display == nullptr)
    {
        delete state;
        return Err(PlatformError::NoDisplayFound);
    }

    state->screen = DefaultScreen(state->display);
    
    return FusionEngine::Ok(PlatformState{state});
}

void FusionEngine::Platform::Update()
{
    // MSG message;
    // while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
    // {
    //     TranslateMessage(&message);
    //     DispatchMessageA(&message);
    // }
    //
    // const HWND focusedWindow = GetFocus();
    // const LONG_PTR lpUserData = GetWindowLongPtr(focusedWindow, GWLP_USERDATA);
    // Window* window = reinterpret_cast<Window*>(lpUserData);
    // Application::Get()->m_FocusedWindow = window;
}

void FusionEngine::Platform::ShutDown()
{
    XCloseDisplay(static_cast<X11State*>(State.InternalState)->display);
    delete static_cast<X11State*>(State.InternalState);
}

FusionEngine::Result<FusionEngine::WindowHandle, FusionEngine::PlatformError>
    FusionEngine::Platform::CreateNativeWindow(FusionEngine::Window* instance)
{
    const X11State* state = static_cast<X11State*>(State.InternalState);

    unsigned long long window = XCreateSimpleWindow(state->display, RootWindow(state->display, state->screen), 10, 10, 800, 600, 1,
        BlackPixel(state->display, state->screen), WhitePixel(state->display, state->screen));

    XSelectInput(state->display, window, ExposureMask | KeyPressMask);
    XMapWindow(state->display, window);

    return FusionEngine::Ok(FusionEngine::WindowHandle{reinterpret_cast<void*>(window), new InputMap()});
}

void FusionEngine::Platform::UpdateNativeWindow(WindowHandle& handle)
{
    const X11State* state = static_cast<X11State*>(State.InternalState);
    unsigned long long window = reinterpret_cast<unsigned long long>(handle.Handle);
    XEvent event;
    while (XNextEvent(state->display, &event))
    {
        switch (event.type)
        {
            case Expose:
                {
                    XWindowAttributes attributes;
                    XGetWindowAttributes(state->display, window, &attributes);
                    EventContext e(Event::WindowResize, handle.Handle);
                    e.Data.uvec2[0] = glm::uvec2(attributes.width, attributes.height);
                    EventSystem::Raise(e);
                } break;
            case KeyPress: 
                {
                    #undef KeyPress
                    const KeyCode keycode = FusionEngine::X11TranslateKeyCode(event.xkey.keycode);
                    handle.InputHandle->UpdateKeyState(keycode, handle.InputHandle->GetKeyState(keycode) == KeyState::Pressed ? KeyState::Down : KeyState::Pressed);
                    EventContext e(Event::KeyPress, handle.Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(keycode);
                    EventSystem::Raise(e);
                } break;
            case KeyRelease:
                {
                    #undef KeyRelease
                    const KeyCode keycode = FusionEngine::X11TranslateKeyCode(event.xkey.keycode);
                    handle.InputHandle->UpdateKeyState(keycode, handle.InputHandle->GetKeyState(keycode) == KeyState::Released ? KeyState::Up : KeyState::Released);
                    EventContext e(Event::KeyRelease, handle.Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(keycode);
                    EventSystem::Raise(e);
                } break;
            case ButtonPress:
                {
                    #undef ButtonPress
                    const MouseCode button = static_cast<MouseCode>(event.xbutton.button);
                    handle.InputHandle->UpdateButtonState(button, handle.InputHandle->GetButtonState(button) == KeyState::Pressed ? KeyState::Down : KeyState::Pressed);
                    EventContext e(Event::ButtonPress, handle.Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(button);
                    EventSystem::Raise(e);
                } break;
            case ButtonRelease:
                {
                    #undef ButtonRelease
                    const MouseCode button = static_cast<MouseCode>(event.xbutton.button);
                    handle.InputHandle->UpdateButtonState(button, handle.InputHandle->GetButtonState(button) == KeyState::Released ? KeyState::Up : KeyState::Released);
                    EventContext e(Event::ButtonRelease, handle.Handle);
                    e.Data.u32[0] = static_cast<uint32_t>(button);
                    EventSystem::Raise(e);
                } break;
            case MotionNotify:
                {
                    const glm::uvec2 pos(event.xmotion.x, event.xmotion.y);
                    handle.InputHandle->UpdateMousePosition(pos);
                    EventContext e(Event::MouseMoved, handle.Handle);
                    e.Data.uvec2[0] = pos;
                    EventSystem::Raise(e);
                } break;
            case ClientMessage:
                {
                    if (event.xclient.data.l[0] == static_cast<long>(XInternAtom(state->display, "WM_DELETE_WINDOW", False)))
                    {
                        EventContext e(Event::WindowClose, handle.Handle);
                        e.Data.ptr[0] = handle.Handle;
                        EventSystem::Raise(e);
                    }
                } break;
            default: break;
        }
    }
}

void FusionEngine::Platform::DestroyNativeWindow(WindowHandle& handle)
{
    XDestroyWindow(static_cast<X11State*>(State.InternalState)->display, reinterpret_cast<unsigned long long>(handle.Handle));
    handle.Handle = nullptr;
    delete handle.InputHandle;
    handle.InputHandle = nullptr;
}

double FusionEngine::Platform::GetAbsTime()
{
    //LARGE_INTEGER now;
    //QueryPerformanceCounter(&now);
    //return g_clockFrequency * static_cast<double>(now.QuadPart);
    return 0.0;
}

void FusionEngine::Platform::SleepM(const double ms)
{
    sleep(ms);
}

namespace FusionEngine
{
    bool Platform::IsKeyDown(KeyCode keycode)
    {
        if (const InputMap* im = Application::Get()->GetCurrentWindow()->GetPlatformHandle().InputHandle)
            return im->GetKeyState(keycode) == KeyState::Down || im->GetKeyState(keycode) == KeyState::Pressed;
        return false;
    }

    bool Platform::IsButtonDown(MouseCode button)
    {
        if (const InputMap* im = Application::Get()->GetCurrentWindow()->GetPlatformHandle().InputHandle)
            return im->GetButtonState(button) == KeyState::Down || im->GetButtonState(button) == KeyState::Pressed;
        return false;
    }

    glm::uvec2 Platform::GetMouse()
    {
        if(const InputMap* im = Application::Get()->GetCurrentWindow()->GetPlatformHandle().InputHandle)
            return im->GetMouse();
        return {0, 0};
    }

    glm::uvec2 Platform::GetMouseDelta()
    {
        if (const InputMap* im = Application::Get()->GetCurrentWindow()->GetPlatformHandle().InputHandle)
            return im->GetMouseDelta();
        return {0, 0};
    }
}

namespace FusionEngine {
    KeyCode X11TranslateKeyCode(const uint32_t key)
    {
        switch (key)
        {
            case XK_Escape: return KeyCode::Escape;
            case XK_Tab: return KeyCode::Tab;
            case XK_Caps_Lock: return KeyCode::CapsLock;
            case XK_Shift_L: return KeyCode::LeftShift;
            case XK_Shift_R: return KeyCode::RightShift;
            case XK_Control_L: return KeyCode::LeftControl;
            case XK_Control_R: return KeyCode::RightControl;
            case XK_Alt_L: return KeyCode::LeftAlt;
            case XK_Alt_R: return KeyCode::RightAlt;
            case XK_Super_L: return KeyCode::LeftSuper;
            case XK_Super_R: return KeyCode::RightSuper;
            case XK_Print: return KeyCode::PrintScreen;
            case XK_space: return KeyCode::Space;
            case XK_apostrophe: return KeyCode::Apostrophe;
            case XK_comma: return KeyCode::Comma;
            case XK_minus: return KeyCode::Minus;
            case XK_period: return KeyCode::Period;
            case XK_slash: return KeyCode::Slash;
            case XK_0: return KeyCode::D0;
            case XK_1: return KeyCode::D1;
            case XK_2: return KeyCode::D2;
            case XK_3: return KeyCode::D3;
            case XK_4: return KeyCode::D4;
            case XK_5: return KeyCode::D5;
            case XK_6: return KeyCode::D6;
            case XK_7: return KeyCode::D7;
            case XK_8: return KeyCode::D8;
            case XK_9: return KeyCode::D9;
            case XK_semicolon: return KeyCode::Semicolon;
            case XK_equal: return KeyCode::Equal;
            case XK_Menu: return KeyCode::Menu;
            case XK_a: return KeyCode::A;
            case XK_b: return KeyCode::B;
            case XK_c: return KeyCode::C;
            case XK_d: return KeyCode::D;
            case XK_e: return KeyCode::E;
            case XK_f: return KeyCode::F;
            case XK_g: return KeyCode::G;
            case XK_h: return KeyCode::H;
            case XK_i: return KeyCode::I;
            case XK_j: return KeyCode::J;
            case XK_k: return KeyCode::K;
            case XK_l: return KeyCode::L;
            case XK_m: return KeyCode::M;
            case XK_n: return KeyCode::N;
            case XK_o: return KeyCode::O;
            case XK_p: return KeyCode::P;
            case XK_q: return KeyCode::Q;
            case XK_r: return KeyCode::R;
            case XK_s: return KeyCode::S;
            case XK_t: return KeyCode::T;
            case XK_u: return KeyCode::U;
            case XK_v: return KeyCode::V;
            case XK_w: return KeyCode::W;
            case XK_x: return KeyCode::X;
            case XK_y: return KeyCode::Y;
            case XK_z: return KeyCode::Z;
            case XK_bracketleft: return KeyCode::LeftBracket;
            case XK_backslash: return KeyCode::Backslash;
            case XK_bracketright: return KeyCode::RightBracket;
            case XK_grave: return KeyCode::GraveAccent;
            case XK_Delete: return KeyCode::Delete;
            case XK_Left: return KeyCode::Left;
            case XK_Right: return KeyCode::Right;
            case XK_Up: return KeyCode::Up;
            case XK_Down: return KeyCode::Down;
            case XK_Insert: return KeyCode::Insert;
            case XK_Home: return KeyCode::Home;
            case XK_End: return KeyCode::End;
            case XK_Page_Up: return KeyCode::PageUp;
            case XK_Page_Down: return KeyCode::PageDown;
            case XK_Num_Lock: return KeyCode::NumLock;
            case XK_KP_Divide: return KeyCode::KP_Divide;
            case XK_KP_Multiply: return KeyCode::KP_Multiply;
            case XK_KP_Subtract: return KeyCode::KP_Subtract;
            case XK_KP_Add: return KeyCode::KP_Add;
            case XK_KP_Enter: return KeyCode::KP_Enter;
            case XK_KP_0: return KeyCode::KP_0;
            case XK_KP_1: return KeyCode::KP_1;
            case XK_KP_2: return KeyCode::KP_2;
            case XK_KP_3: return KeyCode::KP_3;
            case XK_KP_4: return KeyCode::KP_4;
            case XK_KP_5: return KeyCode::KP_5;
            case XK_KP_6: return KeyCode::KP_6;
            case XK_KP_7: return KeyCode::KP_7;
            case XK_KP_8: return KeyCode::KP_8;
            case XK_KP_9: return KeyCode::KP_9;
            case XK_KP_Decimal: return KeyCode::KP_Decimal;
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
