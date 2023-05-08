#pragma once

#ifdef FUSION_ENGINE
    #define FE_API //__declspec(dllexport)
#else
    #define FE_API //__declspec(dllimport)
#endif