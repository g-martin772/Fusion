project "FusionCore"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir "%{BIN}"
    objdir "%{OBJ}"

    files { 
        "./**.h",
        "src/**.h", 
        "src/**.cpp",
    }
    
    defines {
        "FUSION_CORE_MODULE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        ".",
        "src",
        "%{wks.location}/FusionEngine/dependencies/spdlog/include",
        "%{VULKAN_SDK}/Include"
    }

    filter "system:windows"
        systemversion "latest"
        
        defines {
            "FE_WINDOWS"
        }

    filter "system:linux"
        systemversion "latest"
        
        defines {
            "FE_LINUX",
            "FE_X11"
        }

    filter "configurations:Debug"
        defines { "FE_DEBUG" }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { "FE_RELEASE" }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines { "FE_DIST" }
        runtime "Release"
        optimize "On"
