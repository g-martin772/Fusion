#include "fepch.h"
#include "Event.h"

namespace FusionEngine
{
    struct EventEntry
    {
        void* Listener;
        EventCallback Callback;
    };

    static std::unordered_map<uint32_t, std::vector<EventEntry>> s_EventMap;

    bool Event::Register(const uint32_t code, const EventCallback callback, void* instance)
    {
        if (callback == nullptr)
            return false;
        s_EventMap[code].push_back({instance, callback});
        return true;
    }

    bool Event::Register(SystemEvent code, const EventCallback callback, void* instance)
    {
        return Register(static_cast<uint32_t>(code), callback, instance);
    }

    bool Event::Unregister(SystemEvent code, const EventCallback callback, const void* instance)
    {
        return Unregister(static_cast<uint32_t>(code), callback, instance);
    }

    bool Event::Unregister(const uint32_t code, const EventCallback callback, const void* instance)
    {
        const auto it = s_EventMap.find(code);
        if (it != s_EventMap.end())
        {
            auto& entries = it->second;
            for (auto entryIt = entries.begin(); entryIt != entries.end(); ++entryIt)
            {
                if (entryIt->Callback == callback && entryIt->Listener == instance)
                {
                    entries.erase(entryIt);
                    return true;
                }
            }
        }
        return false;
    }

    bool Event::Raise(const EventContext& context)
    {
        const auto it = s_EventMap.find(context.Code);
        if (it != s_EventMap.end())
        {
            for (const auto& entry : it->second)
            {
                if (entry.Callback(context, entry.Listener))
                {
                    return true;
                }
            }
        }
        return false;
    }
}
