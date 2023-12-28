#pragma once

#include "EventContext.h"

namespace FusionEngine
{
    typedef bool (*EventCallback)(const EventContext& context, void* instance);
    #define FE_EVENT_CALLBACK [](const EventContext& context, void* instance)
    
    struct Event
    {
        static bool Register(uint32_t code, EventCallback callback, void* instance = nullptr);
        static bool Register(SystemEvent code, EventCallback callback, void* instance = nullptr);
        static bool Unregister(uint32_t code, EventCallback callback, const void* instance = nullptr);
        static bool Unregister(SystemEvent code, EventCallback callback, const void* instance = nullptr);
        static bool Raise(const EventContext& context);
    };
}
