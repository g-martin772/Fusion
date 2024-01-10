#pragma once

#include "EventContext.h"

namespace FusionEngine
{
    typedef bool (*EventCallback)(const EventContext& context, void* instance);
    #define FE_EVENT_CALLBACK [](const EventContext& context, void* instance)
    #define FE_EVENT_CALLBACK_FN(name) static bool name(const EventContext& context, void* instance)
    
    FE_CORE_MODULE_API struct EventSystem
    {
        FE_CORE_MODULE_API static bool Register(EventCode code, EventCallback callback, void* instance = nullptr);
        FE_CORE_MODULE_API static bool Unregister(EventCode code, EventCallback callback, const void* instance = nullptr);
        FE_CORE_MODULE_API static bool Raise(const EventContext& context);
    };
}
