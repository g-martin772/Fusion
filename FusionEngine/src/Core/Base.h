#pragma once

#ifdef FUSION_ENGINE
    #define FE_API //__declspec(dllexport)
#else
    #define FE_API //__declspec(dllimport)
#endif

namespace FusionEngine
{
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T>
    using Unique = std::unique_ptr<T>;
    
    template<typename T>
    using Weak = std::weak_ptr<T>;
}
