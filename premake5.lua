workspace "Fusion"

architecture "x86_64"
configurations { "Debug", "Release", "Dist" }

flags { "MultiProcessorCompile" }

group "Dependencies"
    include "FusionEngine/Dependencies/GLFW"
    include "FusionEngine/Dependencies/imgui/imgui"
group "Core"
    include "FusionEngine"
    include "FusionForge"
group "Tools"
    include "Sandbox"
group ""
