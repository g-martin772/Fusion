#pragma once

namespace FusionEngine
{
    enum class DrawMode
    {
        None = 0,
        Triangles,
        Lines,
        Points
    };
    
    class RenderApi
    {
    public:
        enum class Api { Vulkan };
        static Api GetApi() { return Api::Vulkan; }
        static Unique<RenderApi> Create();

        virtual void Init() = 0;
        virtual void OnWindowResize(uint32_t width, uint32_t height) = 0;
        virtual void ShutDown() = 0;

        virtual void BeginFrame() = 0;
        virtual void Draw(uint32_t vertexCount) = 0;
        virtual void EndFrame() = 0;
    };
}
