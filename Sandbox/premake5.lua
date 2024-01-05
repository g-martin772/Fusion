project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
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

    VULKAN_SDK = os.getenv("VULKAN_SDK")
    
    includedirs {
        "src",
        "%{wks.location}/FusionEngine/src",
        "%{wks.location}/FusionEngine/dependencies/spdlog/include",
        "%{wks.location}/FusionEngine/dependencies/imgui/imgui",
        "%{wks.location}/FusionEngine/dependencies/EnTT/entt/single_include",
        "%{VULKAN_SDK}/Include"
    }

    links {
        "FusionEngine",
    }

    links {
        "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/VulkanModule/VulkanModule"
    }

    postbuildcommands {
        "{COPY} %{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/VulkanModule %{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Sandbox",
    }

    filter "system:windows"
        systemversion "latest"
    
        defines {
            "FE_WINDOWS"
        }
    
        buildoptions {
            "-fno-ms-extensions",
            "-lstdc++fs"
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
        
        buildoptions {
            "-ftime-report",
            "-v"
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

