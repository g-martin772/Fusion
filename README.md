# Fusion

A C++ Game Engine

Why?

1. I am bored².
2. I am interested in Game Engines.
3. I want to learn stuff.
4. School is boring.

## Building

### Windows

1. Install dependencies:
    ```shell
    winget install Python.Python.3.11
    winget install Git.Git
    winget install GnuWin32.Make
   ```
   Vulkan SDK Version 1.3.231.1 is required. Download [here](https://vulkan.lunarg.com/sdk/home#windows).
2. Clone the repository:
    ```shell
    git clone https://github.com/g-martin772/Fusion --recursive
    ```
3. Run setup script:
    ```shell
    python3 scripts/setup.py
    ```
3. Build:
    * Visual Studio / Rider
      ```shell
         ./premake vs2022 
      ```
      Open your solution and you are ready to go.
    * Make
      ```shell
         ./premake gmake2
         make
      ```
    * VSCode
      ```shell
         ./premake vscode
      ```
      Open the workspace in VSCode and you are ready to go.

### Linux

1. Install dependencies:
    ```shell
    sudo apt install build-essential make python3 git libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
    ```
   Vulkan SDK Version 1.3.231.1 is required. Download [here](https://vulkan.lunarg.com/sdk/home#linux).
2. Clone the repository:
    ```shell
    git clone https://github.com/g-martin772/Fusion --recursive
    ```
3. Run setup script:
    ```shell
    python3 scripts/setup.py
    ```
4. Build:
    ```shell
    ./premake gmake2
    make
    ```
   
