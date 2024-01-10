project "FusionEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    targetdir "%{BIN}"
    objdir "%{OBJ}"

    pchheader "fepch.h"
    pchsource "src/fepch.cpp"

    files { 
        "src/**.h", 
        "src/**.cpp"
    }
    
    defines {
        "FUSION_ENGINE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "src",
        "Modules",
        "dependencies/glfw/include",
        "dependencies/imgui",
        "dependencies/imgui/imgui",
        "dependencies/EnTT/entt/single_include",
        "%{VULKAN_SDK}/Include",
    }
    
    links {
        -- "glfw",
        -- "imgui",
        "FusionCore",
        "VulkanModule",
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

        links {
            "dl",
            "pthread",
            "Xrandr",
            "Xi",
            "Xinerama",
            "Xcursor",
            "Xxf86vm",
            "X11"
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
