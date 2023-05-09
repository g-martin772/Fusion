import os
import subprocess
import platform

from ValidateModules import Validate
Validate()

from Vulkan import CheckVulkan
if not CheckVulkan():
    exit(1)

os.chdir('./../')

subprocess.call(["git", "submodule", "update", "--recursive", "--remote"])
subprocess.call(["git", "submodule", "update", "--init", "--recursive", "--remote"])

print("\nRunning premake...")

if platform.system() == 'Windows':
     subprocess.call([os.path.abspath("./scripts/GenerateVS22.bat"), "nopause"])
elif platform.system() == 'Linux':
    print("missing support, contact dev")
elif platform.system() == 'Darwin':
    print("missing support, contact dev")
else:
    print('Unsupported operating system.')
    exit(1)
