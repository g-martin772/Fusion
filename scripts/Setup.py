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
    print(f"You don't have the correct Vulkan SDK version! (Fusion requires {VULKAN_VERSION})")
    
print(f"Correct Vulkan SDK located at {VULKAN_SDK}")

os.chdir('./../')

print("\nUpdating git submodules ...")

subprocess.call(["git", "submodule", "update", "--recursive", "--remote"])
subprocess.call(["git", "submodule", "update", "--init", "--recursive", "--remote"])

print("\nRunning premake...")

if platform.system() == 'Windows':
     subprocess.call([os.path.abspath("./scripts/GenerateVS22.bat"), "nopause"])
elif platform.system() == 'Linux':
    subprocess.call([os.path.abspath("./scripts/GenerateMakefile.sh"), "nopause"])
else:
    print('What os are you even on at this point xD? Macos or what xD')
    exit(1)
    

