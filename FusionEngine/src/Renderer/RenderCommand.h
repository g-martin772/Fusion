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

        static const Ref<RenderApi>& GetRenderApi();
    private:
        static Ref<RenderApi> s_RenderApi;
    };
}
