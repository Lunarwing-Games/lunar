# Lunar CMakeLists.txt
# (c) 2025 Lunarwing Games
#
# For a guide on configuration, check out CONFIGURING.md!
# This is a monolithic CMakeLists so it can be a bit hard to read at first.

cmake_minimum_required(VERSION 3.20)

project(Lunar
    VERSION 0.1.0
    DESCRIPTION "A game development framework written in C++20."
    LANGUAGES CXX
)

# Stuff with C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fun compile options
option(LUNAR_INTERNAL_LOGGING "Verbose internal logging subsystem." OFF)
option(LUNAR_SCRIPT_MODULE "Lua module." ON)
option(LUNAR_LOGGING_MODULE "Log module for end-users." ON)
option(LUNAR_2DREND_MODULE "2D rendering module." ON)
option(LUNAR_3DREND_MODULE "3D rendering module." ON)
option(LUNAR_ECS_MODULE "Entity component system module." ON)
option(LUNAR_PEDANTIC_COMPILE "Enables highest level of compilation warnings" OFF)
option(LUNAR_ERROR_ON_WARN "Treats all warnings as errors." OFF)
option(LUNAR_BUILD_TESTS "Builds tests for Lunar." OFF)
option(LUNAR_SHADER_MODULE "Enables OpenGL shaders." ON)

if(LUNAR_PEDANTIC_COMPILE)
    message(STATUS "Setting pedantic compilation...")
    if(MSVC)
        add_compile_options(/Wall)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

if(LUNAR_ERROR_ON_WARN)
    message(STATUS "Setting warnings as errors...")
    if(MSVC)
        add_compile_options(/WX)
        add_link_options(/WX)
    else()
        add_compile_options(-Werror)
    endif()
endif()

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/thirdparty/imgui")
set(SDL_DIR "${CMAKE_SOURCE_DIR}/thirdparty/sdl3")
set(LUA_DIR "${CMAKE_SOURCE_DIR}/thirdparty/lua")
find_package(OpenGL REQUIRED)

#
# Files, conditional modules
#

file(GLOB_RECURSE LUNAR_SRC CONFIGURE_DEPENDS "src/*.cpp")
add_library(Lunar STATIC ${LUNAR_SRC})

target_include_directories(Lunar PUBLIC include)

target_compile_definitions(Lunar PUBLIC
    LUNAR_VERSION_STRING="${PROJECT_VERSION}"
)

if(LUNAR_SCRIPT_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_LUA_ENABLED)
    message(STATUS "Lua enabled!")
endif()
if(LUNAR_LOGGING_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_LOG_ENABLED)
    message(STATUS "Logs enabled!")
endif()
if(LUNAR_2DREND_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_2DRENDER_ENABLED)
    message(STATUS "2D Rendering enabled!")
endif()
if(LUNAR_3DREND_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_3DRENDER_ENABLED)
    message(STATUS "3D Rendering enabled!")
endif()
if(LUNAR_ECS_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_ECS_ENABLED)
    message(STATUS "ECS enabled!")
endif()
if(LUNAR_INTERNAL_LOGGING)
    target_compile_definitions(Lunar PUBLIC LUNAR_INTERNAL_LOGS_OK)
    message(WARNING "This is not yet implemented!")
endif()
if(LUNAR_SHADER_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_SHADERS_ENABLED)
    message(STATUS "Shaders enabled! (OpenGL 2, GLSL: 1.2)")
endif()

#
# Compiler / OS specific magic
#

if(WIN32)
    set(LUNAR_PLATFORM "Windows")
elseif(APPLE)
    set(LUNAR_PLATFORM "macOS")
elseif(UNIX)
    set(LUNAR_PLATFORM "Linux")
else()
    set(LUNAR_PLATFORM "Unknown")
endif()
message(STATUS "Target platform: ${LUNAR_PLATFORM}")

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

#
# Common cxx build flags
#

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Disable CRT warnings on MSVC
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if(MSVC)
    # Multi-processor compilation, UTF-8 source, modern compliance
    add_compile_options(/MP /utf-8 /permissive-)
    
    # Not /Wall, that's for pedantic. W3 is fine enough!
    add_compile_options(/W3)
    if(LUNAR_ERROR_ON_WARN)
        add_compile_options(/WX)
    endif()
else()
    # Okay this looks like a lot but it's still easier than ISO C++ 20!
    add_compile_options(
        -Wall -Wextra -Wconversion -Wshadow -Wformat=2 
        -Wno-unused-parameter -Wundef -Wpointer-arith
        -Wswitch-enum -Wcast-align -Wmissing-declarations
        -Wlogical-op -Wredundant-decls -Wformat-strings
        -Wnull-dereference
        )
    if(LUNAR_ERROR_ON_WARN)
        add_compile_options(-Werror)
    endif()

    target_compile_options(Lunar PRIVATE    -fno-rtti -fno-exceptions)
    add_compile_options(-fdiagnostics-color=always)
endif()

message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

# Inject compiler info into source code
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_definitions(LUNAR_COMPILER "Clang")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_definitions(LUNAR_COMPILER "GCC")
elseif(MSVC)
    add_compile_definitions(LUNAR_COMPILER "MSVC")
else()
    add_compile_definitions(LUNAR_COMPILER "Unknown/Other")
endif()

# Debug / Rel specifics
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Applying Debug build settings...")
    if(NOT MSVC)
        add_compile_options(-O0 -g3)
    else()
        add_compile_options(/Od /Zi)
    endif()
else()
    message(STATUS "Applying Release build settings...")
    if(NOT MSVC)
        add_compile_options(-O2)
    else()
        add_compile_options(/O2)
    endif()
endif()

if(LUNAR_PEDANTIC_COMPILE AND NOT MSVC)
    add_compile_options(-pedantic)
endif()

#
# Dependency hell
#

find_package(OpenGL REQUIRED)

set(SDL3_INC "${SDL_DIR}/include")

set(_SDL3_CANDIDATES
    "${SDL_DIR}/libSDL3.dylib"
    "${SDL_DIR}/SDL3.dll"
    "${SDL_DIR}/SDL3.lib"
    "${SDL_DIR}/libSDL3.so"
)
set(_SDL3_LIB "")
foreach(cand IN LISTS _SDL3_CANDIDATES)
    if(EXISTS "${cand}")
        set(_SDL3_LIB "${cand}")
        break()
    endif()
endforeach()

if(NOT _SDL3_LIB)
    message(FATAL_ERROR "Could not find a prebuilt SDL3 library in ${SDL_DIR}. "
                        "Expected one of: libSDL3.dylib, SDL3.dll/.lib, libSDL3.so")
endif()

add_library(SDL3::SDL3 UNKNOWN IMPORTED)
set_target_properties(SDL3::SDL3 PROPERTIES
    IMPORTED_LOCATION "${_SDL3_LIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INC}"
)

if(APPLE AND _SDL3_LIB MATCHES "\\.dylib$")
    # nothing extra needed; CMake will link the dylib path
endif()

set(LUA_INC "${LUA_DIR}")
set(_LUA_CANDIDATES
    "${LUA_DIR}/liblua.dylib"
    "${LUA_DIR}/lua.dll"
    "${LUA_DIR}/lua.lib"
    "${LUA_DIR}/liblua.so"
)
set(_LUA_LIB "")
foreach(cand IN LISTS _LUA_CANDIDATES)
    if(EXISTS "${cand}")
        set(_LUA_LIB "${cand}")
        break()
    endif()
endforeach()

if(LUNAR_SCRIPT_MODULE AND NOT _LUA_LIB)
    message(FATAL_ERROR "LUNAR_SCRIPT_MODULE=ON but no Lua library found in ${LUA_DIR}. "
                        "Expected one of: liblua.dylib, lua.dll/.lib, liblua.so")
endif()

if(_LUA_LIB)
    add_library(Lua::Lua UNKNOWN IMPORTED)
    set_target_properties(Lua::Lua PROPERTIES
        IMPORTED_LOCATION "${_LUA_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${LUA_INC}"
    )
endif()

set(IMGUI_CORE_SRC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
)
set(IMGUI_BACKENDS_SRC "")
if(LUNAR_2DREND_MODULE OR LUNAR_3DREND_MODULE)
    list(APPEND IMGUI_BACKENDS_SRC
        "${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp"
    )
endif()

add_library(imgui STATIC ${IMGUI_CORE_SRC} ${IMGUI_BACKENDS_SRC})
add_library(ImGui::imgui ALIAS imgui)

target_include_directories(imgui PUBLIC
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
)

if(LUNAR_2DREND_MODULE OR LUNAR_3DREND_MODULE)
    target_link_libraries(imgui PRIVATE SDL3::SDL3 OpenGL::GL)
    if(APPLE)
    endif()
endif()

target_link_libraries(Lunar
    PRIVATE
        SDL3::SDL3
        ImGui::imgui
)

if(LUNAR_2DREND_MODULE OR LUNAR_3DREND_MODULE)
    target_link_libraries(Lunar PRIVATE OpenGL::GL)
endif()

if(LUNAR_SCRIPT_MODULE AND TARGET Lua::Lua)
    target_link_libraries(Lunar PRIVATE Lua::Lua)
endif()

if(UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Lunar PRIVATE Threads::Threads)
endif()


#
# Actual compilation/linking steps!
#

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LunarConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LunarConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LunarConfigVersion.cmake"
    DESTINATION lib/cmake/Lunar
)

message(STATUS "")
message(STATUS "====================================")
message(STATUS " Lunar Configuration Summary")
message(STATUS "====================================")
message(STATUS " Version:            ${PROJECT_VERSION}")
message(STATUS " Platform:           ${LUNAR_PLATFORM}")
message(STATUS " Compiler:           ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS " Build Type:         ${CMAKE_BUILD_TYPE}")
message(STATUS " Modules:")
message(STATUS "   Lua Script:       ${LUNAR_SCRIPT_MODULE}")
message(STATUS "   Logging:          ${LUNAR_LOGGING_MODULE}")
message(STATUS "   2D Renderer:      ${LUNAR_2DREND_MODULE}")
message(STATUS "   3D Renderer:      ${LUNAR_3DREND_MODULE}")
message(STATUS "   ECS:              ${LUNAR_ECS_MODULE}")
message(STATUS "   Shaders:          ${LUNAR_SHADER_MODULE}")
message(STATUS "====================================")
message(STATUS "If shaders are ON, use GLSL Version 1.2!")
message(STATUS "")