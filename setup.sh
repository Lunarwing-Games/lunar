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

echo "Complete!"
echo