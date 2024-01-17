project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir "%{BIN}"
    objdir "%{OBJ}"
    
    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
        "FE_RENDER_BACKEND_VULKAN"
    }

    includedirs {
        "src",
        "%{wks.location}/FusionEngine/src",
        "%{wks.location}/FusionEngine/Modules",
        "%{wks.location}/FusionEngine/dependencies/spdlog/include",
        "%{wks.location}/FusionEngine/dependencies/imgui/imgui",
        "%{wks.location}/FusionEngine/dependencies/EnTT/entt/single_include",
        "%{VULKAN_SDK}/Include"
    }

    links {
        "FusionEngine"
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

