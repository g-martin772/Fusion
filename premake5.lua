workspace "Fusion"

architecture "x86_64"
configurations { "Debug", "Release", "Dist" }

flags { "MultiProcessorCompile" }

include "FusionEngine"
include "Sandbox"
include "FusionForge"
