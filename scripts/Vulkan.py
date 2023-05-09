import os
import subprocess
import sys
from pathlib import Path
import requests
import sys
import time
from fake_useragent import UserAgent
from io import BytesIO
from urllib.request import urlopen

VULKAN_SDK = os.environ.get('VULKAN_SDK')
VULKAN_SDK_INSTALLER_URL = 'https://sdk.lunarg.com/sdk/download/1.3.231.1/windows/vulkan_sdk.exe'
VULKAN_VERSION = "1.3.231.1"
VULKAN_SDK_EXE_PATH = 'FusionEngine/dependencies/VulkanSDK/VulkanSDK.exe'

def DownloadFile(url, filepath):
    with open(filepath, 'wb') as f:
        ua = UserAgent()
        headers = {'User-Agent': ua.chrome}
        response = requests.get(url, headers=headers, stream=True)
        total = response.headers.get('content-length')

        if total is None:
            f.write(response.content)
        else:
            downloaded = 0 
            total = int(total)
            startTime = time.time()
            for data in response.iter_content(chunk_size=max(int(total/1000), 1024*1024)):
                downloaded += len(data)
                f.write(data)
                done = int(50*downloaded/total)
                percentage = (downloaded / total) * 100
                elapsedTime = time.time() - startTime
                avgKBPerSecond = (downloaded / 1024) / elapsedTime
                avgSpeedString = '{:.2f} KB/s'.format(avgKBPerSecond)
                if (avgKBPerSecond > 1024):
                    avgMBPerSecond = avgKBPerSecond / 1024
                    avgSpeedString = '{:.2f} MB/s'.format(avgMBPerSecond)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, avgSpeedString))
                sys.stdout.flush()
    sys.stdout.write('\n')

def InstallVulkanSDK():
    print('Downloading {} to {}'.format(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH))
    DownloadFile(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH)
    print("Done!")
    print("Running Vulkan SDK installer...")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print("Re-run this script after installation")
    
def CheckVulkan():
    print(f"Checking for vulkan version {VULKAN_VERSION}")
    
    if not os.path.isdir("FusionEngine/dependencies/VulkanSDK"):
        os.makedirs("FusionEngine/dependencies/VulkanSDK")
    
    if (VULKAN_SDK is None):
       print("You don't have the Vulkan SDK installed!")
       InstallVulkanSDK()
       return False
    elif (VULKAN_VERSION not in VULKAN_SDK):
        print(f"Located Vulkan SDK at {VULKAN_SDK}")
        print(f"You don't have the correct Vulkan SDK version! (Fusion requires {NUT_VULKAN_VERSION})")
        InstallVulkanSDK()
        return False
        
    print(f"Correct Vulkan SDK located at {VULKAN_SDK}")
    return True
