#pragma once

#include "EventContext.h"

namespace FusionEngine
{
    typedef bool (*EventCallback)(const EventContext& context, void* instance);
    #define FE_EVENT_CALLBACK [](const EventContext& context, void* instance)
    
    struct EventSystem
    {
        static bool Register(EventCode code, EventCallback callback, void* instance = nullptr);
        static bool Unregister(EventCode code, EventCallback callback, const void* instance = nullptr);
        static bool Raise(const EventContext& context);
    };
}
