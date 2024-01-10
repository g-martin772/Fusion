project "VulkanModule"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    targetdir "%{BIN}"
    objdir "%{OBJ}"

    files { 
        "src/**.h", 
        "src/**.cpp",
    }
    
    defines {
        "VULKAN_MODULE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "src",
        "%{VULKAN_SDK}/Include"
    }

    filter "system:windows"
        systemversion "latest"
        
        defines {
            "FE_WINDOWS"
        }

        links {
            "%{VULKAN_SDK}/Lib/vulkan-1.lib",
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
            "%{VULKAN_SDK}/lib/vulkan",
            "%{VULKAN_SDK}/lib/shaderc_shared",
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
