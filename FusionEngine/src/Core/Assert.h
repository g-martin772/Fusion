#pragma once

#include "Core/Log.h"

#define FE_ASSERT(x, ...) { if(!(x)) {FE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define FF_ASSERT(x, ...) { if(!(x)) {FF_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define FG_ASSERT(x, ...) { if(!(x)) {FG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
