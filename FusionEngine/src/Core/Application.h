#pragma once
#include <iostream>

#include "Base.h"

namespace FusionEngine
{
    FE_API class Application
    {
    public:
        void Init();
        void Run();
        void Shutdown();
    private:
        bool m_Running = true;
    };
}