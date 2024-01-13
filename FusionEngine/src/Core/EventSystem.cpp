#include "fepch.h"
#include "EventSystem.h"

#include "Base.h"

namespace FusionEngine
{
    struct EventContext;

    struct EventEntry
    {
        void* Listener;
        EventCallback Callback;
    };

    static UMap<EventGroup, UMap<EventCode, List<EventEntry>>> s_EventMap;

    bool EventSystem::Register(const EventCode code, const EventCallback callback, void* instance)
    {
        if (callback == nullptr)
            return false;

        s_EventMap[EVENT_GROUP(code)][code].push_back({instance, callback});
        auto asd = s_EventMap;
        return true;
    }

    bool EventSystem::Unregister(const EventCode code, const EventCallback callback, const void* instance)
    {
        const auto groupIt = s_EventMap.find(EVENT_GROUP(code));
        if (groupIt == s_EventMap.end())
            return false;
        
        const auto eventIt = groupIt->second.find(code);
        if (eventIt == groupIt->second.end())
            return false;
        
        auto& entries = eventIt->second;
        for (auto entryIt = entries.begin(); entryIt != entries.end(); ++entryIt)
        {
            if (entryIt->Callback == callback && entryIt->Listener == instance)
            {
                entries.erase(entryIt);
                return true;
            }
        }

        return false;
    }

    bool EventSystem::Raise(const EventContext& context)
    {
        auto g = EVENT_GROUP(context.Code);
        const auto groupIt = s_EventMap.find(g);
        if (groupIt == s_EventMap.end())
            return false;
        
        const auto eventIt = groupIt->second.find(context.Code);
        if (eventIt == groupIt->second.end())
            return false;

        for (const auto& entry : eventIt->second)
        {
            if (entry.Callback(context, entry.Listener))
            {
                return true;
            }
        }
        
        return false;
    }
}
