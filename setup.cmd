:: Setup script for Windows x64 environments.
:: the '.deps' directory is temporary and should be deleted afterwards

@echo off
setlocal enabledelayedexpansion

if not exist ".deps" mkdir ".deps"
if not exist "thirdparty" mkdir "thirdparty"

rem Phase 1: cloning and building SDL3
rem          destination: .\thirdparty\sdl3

git clone https://github.com/libsdl-org/SDL.git --depth 1 .deps\sdl
cd .deps\sdl
cmake -S . -B build -G Ninja
cd build
ninja
xcopy /E /I /Y ".\SDL3.dll" "..\..\..\thirdparty\sdl3\SDL3.dll"
xcopy /E /I /Y ".\SDL3.lib" "..\..\..\thirdparty\sdl3\SDL3.lib"
xcopy /E /I /Y "..\include\SDL" "..\..\..\thirdparty\sdl3\include\SDL"
cd ..\..\..

rem Phase 2: cloning and copying imgui files
rem          destination: .\thirdparty\imgui

git clone https://github.com/ocornut/imgui.git --depth 1 .deps\imgui
cd .deps\imgui
xcopy /E /I /Y ".\*.cpp" "..\..\thirdparty\imgui\"
xcopy /E /I /Y ".\*.h" "..\..\thirdparty\imgui\"
xcopy /E /I /Y ".\backends" "..\..\thirdparty\imgui\backends"
cd ..\..

rem Phase 3: cloning and building Lua
rem          this gets a little bit hacky...
rem          destination: .\thirdparty\lua

git clone https://github.com/lua/lua.git --depth 1 .deps\lua
cd .deps\lua
cl /O2 /W3 /LD l*.c -Dlua_build_as_DLL -DLUA_BUILD_AS_DLL -o lua.dll
xcopy /E /I /Y ".\lua.h" "..\..\thirdparty\lua\lua.h"
xcopy /E /I /Y ".\lauxlib.h" "..\..\thirdparty\lua\lauxlib.h"
xcopy /E /I /Y ".\lualib.h" "..\..\thirdparty\lua\lualib.h"
xcopy /E /I /Y ".\lua.dll" "..\..\thirdparty\lua\lua.dll"
xcopy /E /I /Y ".\lua.lib" "..\..\thirdparty\lua\lua.lib"
