import os
import subprocess
import platform

VULKAN_SDK = os.environ.get('VULKAN_SDK')
VULKAN_VERSION = "1.3.231.1"

print(f"Checking for vulkan version {VULKAN_VERSION}")

if (VULKAN_SDK is None):
   print("You don't have the Vulkan SDK installed!")
   exit(1)
elif (VULKAN_VERSION not in VULKAN_SDK):
    print(f"Located Vulkan SDK at {VULKAN_SDK}")
    print(f"You don't have the correct Vulkan SDK version! (Fusion requires version {VULKAN_VERSION})")
    
print(f"Correct Vulkan SDK located at {VULKAN_SDK}")

if "scripts" in os.getcwd():
    os.chdir('./../')
    
os.environ["FUSION_DIR"] = os.getcwd()

print("\nUpdating git submodules ...")

subprocess.call(["git", "submodule", "update", "--recursive", "--remote"])
subprocess.call(["git", "submodule", "update", "--init", "--recursive", "--remote"])

if platform.system() == "Windows":
    if not os.path.isfile("premake.exe"):
        os.link("scripts/Dependencies/premake-5.0.0-beta2-windows/premake5.exe", "premake.exe")
elif platform.system() == "Linux":
    if not os.path.isfile("premake"):
        os.link("scripts/Dependencies/premake-5.0.0-beta2-linux/premake5", "premake")