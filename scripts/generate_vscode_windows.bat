@echo off
pushd %~dp0\..\
call scripts\Dependencies\premake-5.0.0-beta2-windows\premake5.exe gmake2
call scripts\Dependencies\premake-5.0.0-beta2-windows\premake5.exe vscode
popd
pause