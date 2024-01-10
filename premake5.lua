require("scripts/Dependencies/vscode")

BIN = "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
OBJ = "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VULKAN_SDK = os.getenv("VULKAN_SDK")

workspace "Fusion"

architecture "x86_64"
configurations { "Debug", "Release", "Dist" }

flags { "MultiProcessorCompile" }

group "Dependencies"
    include "FusionEngine/Dependencies/GLFW"
    include "FusionEngine/Dependencies/imgui/imgui"
group "Engine"
    include "FusionEngine"
    include "FusionForge"
group "Tests"
    include "Sandbox"
group "Modules"
    include "FusionEngine/Modules/Vulkan"
    include "FusionEngine/Modules/Core"
group ""
