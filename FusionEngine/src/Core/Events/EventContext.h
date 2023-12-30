#pragma once

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

    #define DEFINE_EVENT(name, group, id) constexpr EventCode name = static_cast<EventCode>(group) | (id)  // NOLINT(bugprone-macro-parentheses)

    namespace Event
    {
        DEFINE_EVENT(ApplicationShutdown, EventGroup::Application, 0x01);
        DEFINE_EVENT(WindowResize, EventGroup::Window, 0x02);
        DEFINE_EVENT(WindowClose, EventGroup::Window, 0x02);
        DEFINE_EVENT(WindowMove, EventGroup::Window, 0x03);
    };

    #define EVENT_GROUP(event) static_cast<EventGroup>(static_cast<EventCode>(event) & 0xFFFF0000)
    #define EVENT_ID(event) (static_cast<EventCode>(event) & 0x0000FFFF)

    // TODO: Input events and EventContextData parser thing
    enum class SystemEvents : uint32_t
    {
        ApplicationShutdown = 0x01,
        WindowResize = 0x02,
        WindowMove = 0x03,
        WindowFocus = 0x04,
        WindowLostFocus = 0x05,
        WindowClose = 0x06,
        WindowMinimize = 0x07,
        WindowMaximize = 0x08,
        WindowRestore = 0x09,
        KeyPress = 0x10,
        KeyRelease = 0x11,
        ButtonPress = 0x12,
        ButtonRelease = 0x13,
        MouseMoved = 0x14,
        MouseScrolled = 0x15,
        Max = 0xff
    };

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

            glm::vec2 uvec2[2];
            glm::vec3 uvec3;
            glm::vec4 uvec4;
            glm::mat2 umat2;

            glm::vec2 ivec2[2];
            glm::vec3 ivec3;
            glm::vec4 ivec4;
            glm::mat2 imat2;

            void* ptr[2];

            char c[16];

            EventContextData() : u64{0, 0}
            {
            }
        } Data;

        EventCode Code;
        void* Sender;

        EventContext() = default;

        EventContext(const EventCode code, void* sender)
            : Code(code), Sender(sender)
        {
        }
        
    };
}
