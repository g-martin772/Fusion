workspace "Fusion"

architecture "x86_64"
configurations { "Debug", "Release", "Dist" }

flags { "MultiProcessorCompile" }

group "Dependencies"
include "FusionEngine/Dependencies/GLFW"
group "Core"
include "FusionEngine"
include "FusionForge"
group "Tool"
include "Sandbox"
group ""
