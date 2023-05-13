#pragma once
#include "RenderApi.h"

namespace FusionEngine
{
    class RenderCommand
    {
    public:
        static void Init();
        static void ShutDown();
        static void Render();

        static void ResizeWindow(uint32_t width, uint32_t height);

        static const Ref<RenderApi>& GetRenderApi();
    private:
        static Ref<RenderApi> s_RenderApi;
    };
}
