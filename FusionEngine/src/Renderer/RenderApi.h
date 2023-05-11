#pragma once

namespace FusionEngine
{
    class RenderApi
    {
    public:
        enum class Api { Vulkan };
        static Api GetApi() { return Api::Vulkan; }
        static Unique<RenderApi> Create();

        virtual void Init() = 0;
        virtual void OnWindowResize(uint32_t width, uint32_t height) = 0;
        virtual void ShutDown() = 0;

        virtual void Render() = 0;
    };
}
