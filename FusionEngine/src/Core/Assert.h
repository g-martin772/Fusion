#pragma once

#include "Core/Log.h"

#include <signal.h>

#ifdef FE_WINDOWS
    #define FE_BREAK __debugbreak()
#else
    #define FE_BREAK raise(SIGTRAP)
#endif

#ifdef FE_DEBUG
    #define FE_ENABLE_ASSERTS
#endif

#ifdef FE_ENABLE_ASSERTS
#define FE_ASSERT(x, ...) { if(!(x)) {FE_ERROR("Assertion Failed: {0}", __VA_ARGS__); FE_BREAK;}}
#define FF_ASSERT(x, ...) { if(!(x)) {FF_ERROR("Assertion Failed: {0}", __VA_ARGS__); FE_BREAK;}}
#define FG_ASSERT(x, ...) { if(!(x)) {FG_ERROR("Assertion Failed: {0}", __VA_ARGS__); FE_BREAK;}}
#else
#define FE_ASSERT(x, ...)
#define FF_ASSERT(x, ...)
#define FG_ASSERT(x, ...)
#endif