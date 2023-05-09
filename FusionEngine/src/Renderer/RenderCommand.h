#pragma once
#include "RenderApi.h"

namespace FusionEngine
{
    class RenderCommand
    {
    public:
        static void Init();
        static void ShutDown();
    private:
        static Unique<RenderApi> s_RenderApi;
    };
}
