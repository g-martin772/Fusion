project "FusionEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    pchheader "fepch.h"
    pchsource "src/fepch.cpp"

    files { 
        "src/**.h", 
        "src/**.cpp",
        "src/**.hpp"
    }
    
    defines {
        "FUSION_ENGINE",
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }
    
    VULKAN_SDK = os.getenv("VULKAN_SDK")

    includedirs {
        "src",
        "dependencies/spdlog/include",
        "dependencies/glfw/include",
        "%{VULKAN_SDK}/Include"
    }
    
    links {
        "glfw",
        "%{VULKAN_SDK}/Lib/vulkan-1.lib"
    }

    -- postbuildcommands {
    --     ("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Sandbox")
    -- }

    filter "system:windows"
        systemversion "latest"
        
        defines {
            "FE_WINDOWS"
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

