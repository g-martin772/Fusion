project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp"
    }
    
    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }
    
    includedirs {
        "src",
        "%{wks.location}/FusionEngine/src",
        "%{wks.location}/FusionEngine/dependencies/spdlog/include"
    }

    links {
        "FusionEngine"
    }
    
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

