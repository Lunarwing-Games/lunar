#!/usr/bin/env bash

# Setup for UNIX-Like systems (Linux, macOS)

set -euo pipefail
IFS=$'\n\t'

mkdir -p .deps
mkdir -p thirdparty

# Phase 1 -> SDL
git clone --depth 1 https://github.com/libsdl-org/SDL.git .deps/sdl
pushd .deps/sdl > /dev/null

cmake -S . -B build -G Ninja
cmake --build build

mkdir -p ../../thirdparty/sdl3/include
cp -v build/libSDL3.so ../../thirdparty/sdl3/SDL3.so 2>/dev/null || true
cp -v build/SDL3.a ../../thirdparty/sdl3/SDL3.a 2>/dev/null || true
cp -v build/SDL3.lib ../../thirdparty/sdl3/SDL3.lib 2>/dev/null || true
cp -v build/libSDL3.dylib ../../thirdparty/sdl3/libSDL3.dylib 2>/dev/null || true
cp -rv include/SDL3 ../../thirdparty/sdl3/include/

popd > /dev/null

# Phase 2 -> imgui
git clone --depth 1 https://github.com/ocornut/imgui.git .deps/imgui
mkdir -p thirdparty/imgui/backends

cp -v .deps/imgui/*.cpp thirdparty/imgui/ || true
cp -v .deps/imgui/*.h thirdparty/imgui/ || true
cp -rv .deps/imgui/backends/* thirdparty/imgui/backends/ || true

# Phase 3 -> Lua
mkdir -p thirdparty/lua

if [[ "$(uname)" == "Darwin" ]]; then
    echo "macOS — using Homebrew Lua..."
    if ! command -v brew >/dev/null 2>&1; then
        echo "Homebrew not found! Please install it: https://brew.sh"
        exit 1
    fi

    brew install lua || true

    LUA_PREFIX=$(brew --prefix lua)
    cp -v "$LUA_PREFIX/include/lua/lua.h" thirdparty/lua/
    cp -v "$LUA_PREFIX/include/lua/lualib.h" thirdparty/lua/
    cp -v "$LUA_PREFIX/include/lua/lauxlib.h" thirdparty/lua/
    cp -v "$LUA_PREFIX/include/lua/luaconf.h" thirdparty/lua
    sudo cp -v "$LUA_PREFIX/lib/liblua.dylib" thirdparty/lua/

else
    echo "Linux — cloning and building Lua..."
    git clone --depth 1 https://github.com/lua/lua.git .deps/lua
    pushd .deps/lua > /dev/null

    make
    cp -v lua.h lauxlib.h lualib.h luaconf.h ../../thirdparty/lua/
    cp -v liblua.so ../../thirdparty/lua/

    popd > /dev/null
fi

echo "Complete!"
echo