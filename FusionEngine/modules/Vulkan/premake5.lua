project "VulkanModule"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    files { 
        "src/**.h", 
        "src/**.cpp",
        "src/**.hpp",
        "include/**.h",
        "include/**.cpp",
        "include/**.hpp",
    }
    
    defines {
        "VULKAN_MODULE",
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    VULKAN_SDK = os.getenv("VULKAN_SDK")

    includedirs {
        "src",
        "include",
        "dependencies/spdlog/include",
        "%{wks.location}/FusionEngine/src",
        "%{VULKAN_SDK}/Include"
    }

    -- postbuildcommands {
    --     ("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Sandbox")
    -- }

    filter "system:windows"
        systemversion "latest"
        
        defines {
            "FE_WINDOWS"
        }

        links {
            "%{VULKAN_SDK}/Lib/vulkan-1.lib",
        }

        buildoptions {
            "-fno-ms-extensions",
            "-lstdc++fs"
        }

        filter {"configurations:Debug", "system:windows"}
            links {
                "%{VULKAN_SDK}/Lib/shaderc_combinedd.lib"
            }
        
        filter {"configurations:Release", "system:windows"}
            links {
                "%{VULKAN_SDK}/Lib/shaderc_combined.lib"
            }

        filter {"configurations:Dist", "system:windows"}
            links {
                "%{VULKAN_SDK}/Lib/shaderc_combined.lib"
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

        links {
            "%{VULKAN_SDK}/lib/vulkan",
            "%{VULKAN_SDK}/lib/shaderc_shared",
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
