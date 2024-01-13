#pragma once

#include "Base.h"

namespace FusionEngine
{
    class Log
    {
    public:
        static void Init();
    }; 
}

#define FE_TRACE(...)
#define FE_INFO(...)
#define FE_WARN(...)
#define FE_ERROR(...)
#define FE_FATAL(...)