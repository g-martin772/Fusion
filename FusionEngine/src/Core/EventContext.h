#pragma once

#include <cassert>
#include <glm/glm.hpp>

namespace FusionEngine
{
    using EventCode = uint32_t;

    #define DEFINE_EVENT_GROUP(name, id) name = (id) << 16 
    
    enum class EventGroup : EventCode
    {
        DEFINE_EVENT_GROUP(None, 0), 
        DEFINE_EVENT_GROUP(System, 1),
        DEFINE_EVENT_GROUP(Window, 2),
        DEFINE_EVENT_GROUP(Application, 3),
        DEFINE_EVENT_GROUP(Input, 4),
        DEFINE_EVENT_GROUP(Network, 5),
        DEFINE_EVENT_GROUP(Audio, 6),
        DEFINE_EVENT_GROUP(Graphics, 7),
        DEFINE_EVENT_GROUP(Physics, 8),
        DEFINE_EVENT_GROUP(Scripting, 9),
        DEFINE_EVENT_GROUP(Device, 10),
        DEFINE_EVENT_GROUP(Debug, 11),
        DEFINE_EVENT_GROUP(Game, 12)
    };

    #define DEFINE_EVENT(name, group) constexpr EventCode name = (static_cast<EventCode>(group) | (__COUNTER__))

    namespace Event
    {
        DEFINE_EVENT(ApplicationShutdown, EventGroup::Application); // None
        DEFINE_EVENT(WindowResize, EventGroup::Window); // Data.uvec2[0] -> NewWindowSize
        DEFINE_EVENT(WindowClose, EventGroup::Window); // Data.ptr[0} -> Window*
        //DEFINE_EVENT(WindowMove, EventGroup::Window); // Data.uvec2[0] -> NewWindowPos
        //DEFINE_EVENT(WindowFocus, EventGroup::Window); // Data.ptr[0} -> Window*
        //DEFINE_EVENT(WindowLostFocus, EventGroup::Window); // Data.ptr[0} -> Window*
        //DEFINE_EVENT(WindowMinimize, EventGroup::Window); // Data.ptr[0} -> Window*
        //DEFINE_EVENT(WindowMaximize, EventGroup::Window); // Data.ptr[0} -> Window*
        //DEFINE_EVENT(WindowRestore, EventGroup::Window); // Data.ptr[0} -> Window*
        DEFINE_EVENT(KeyPress, EventGroup::Application); // Data.u32[0] -> KeyCode
        DEFINE_EVENT(KeyRelease, EventGroup::Application); // Data.u32[0] -> KeyCode
        DEFINE_EVENT(ButtonPress, EventGroup::Application); // Data.u32[0] -> MouseCode
        DEFINE_EVENT(ButtonRelease, EventGroup::Application); // Data.u32[0] -> MouseCode
        DEFINE_EVENT(MouseMoved, EventGroup::Application); // Data.uvec2[0] -> NewMousePos
        DEFINE_EVENT(MouseScrolled, EventGroup::Application); // Data.u32[0] -> delta Z
    }

    #define EVENT_GROUP(event) static_cast<EventGroup>(static_cast<EventCode>(event) & 0xFFFF0000)
    #define EVENT_ID(event) (static_cast<EventCode>(event) & 0x0000FFFF)
    
    struct EventContext
    {
        union EventContextData // 128bits
        {
            uint64_t u64[2];
            int64_t i64[2];
            double f64[2];

            uint32_t u32[4];
            int32_t i32[4];
            float f32[4];

            uint16_t u16[8];
            int16_t i16[8];

            uint8_t u8[16];
            int8_t i8[16];

            glm::vec2 vec2[2];
            glm::vec3 vec3;
            glm::vec4 vec4;
            glm::mat2 mat2;

            glm::uvec2 uvec2[2];
            glm::uvec3 uvec3;
            glm::uvec4 uvec4;

            glm::ivec2 ivec2[2];
            glm::ivec3 ivec3;
            glm::ivec4 ivec4;

            void* ptr[2];

            char c[16];

            EventContextData() : u64{0, 0} {}
        } Data;
        
        EventCode Code;
        void* Sender;

        EventContext(const EventCode code, void* sender)
            : Code(code), Sender(sender) {}

        EventContext(EventContextData data, const EventCode code, void* sender)
            : Data(data), Code(code), Sender(sender) {}
    };
}
