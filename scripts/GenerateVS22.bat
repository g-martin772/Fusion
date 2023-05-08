@echo off
pushd %~dp0\..\
call scripts\Dependencies\premake-5.0.0-beta2-windows\premake5.exe vs2022
popd