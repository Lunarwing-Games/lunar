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
option(LUNAR_SCRIPT_MODULE "Lua module." OFF)
option(LUNAR_LOGGING_MODULE "Log module for end-users." ON)
option(LUNAR_2DREND_MODULE "2D rendering module." ON)
option(LUNAR_3DREND_MODULE "3D rendering module." ON)
option(LUNAR_ECS_MODULE "Entity component system module." ON)
option(LUNAR_PEDANTIC_COMPILE "Enables highest level of compilation warnings" OFF)
option(LUNAR_ERROR_ON_WARN "Treats all warnings as errors." OFF)
option(LUNAR_BUILD_TESTS "Builds tests for Lunar." OFF)
option(LUNAR_BUILD_WORKERS "1" CACHE STRING "Sets the amount of threads to use for compilation." )

if(LUNAR_BUILD_WORKERS GREATER 1)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${LUNAR_BUILD_WORKERS})
endif()

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

add_subdirectory(thirdparty/SDL3)
add_subdirectory(thirdparty/lua)
add_subdirectory(thirdparty/dear_imgui)
add_subdirectory(thirdparty/glad)

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
endif()
if(LUNAR_LOGGING_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_LOG_ENABLED)
endif()
if(LUNAR_2DREND_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_2DRENDER_ENABLED)
endif()
if(LUNAR_3DREND_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_3DRENDER_ENABLED)
endif()
if(LUNAR_ECS_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_ECS_ENABLED)
endif()
if(LUNAR_INTERNAL_LOGGING)
    target_compile_definitions(Lunar PUBLIC LUNAR_INTERNAL_LOGS_OK)
    message(WARNING "This is not yet implemented!")
endif()
if(LUNAR_VENDOR_ONLY)
    message(STATUS "Vendor only mode enabled.")
endif()
if(LUNAR_BUILD_TESTS)
    message(WARNING "This is not supported!")
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
# Actual compilation/linking steps!
#

if(TARGET SDL3::SDL3)
    target_link_libraries(Lunar PUBLIC SDL3::SDL3)
elseif(TARGET SDL3)
    target_link_libraries(Lunar PUBLIC SDL3)
endif()

if(LUNAR_SCRIPT_MODULE AND TARGET lua)
    target_link_libraries(Lunar PUBLIC lua)
endif()

if(TARGET imgui)
    target_link_libraries(Lunar PUBLIC imgui)
endif()

if(TARGET glad)
    target_link_libraries(Lunar PUBLIC glad)
endif()

install(TARGETS Lunar
    EXPORT LunarTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    # RUNTIME DESTINATION bin
    # We don't need a runtime, this is a lib, not an app
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT LunarTargets
    FILE LunarTargets.cmake
    NAMESPACE Lunar::
    DESTINATION lib/cmake/Lunar
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LunarConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LunarConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LunarConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LunarConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LunarConfigVersion.cmake"
    DESTINATION lib/cmake/Lunar
)

message(STATUS "")
message(STATUS "====================================")
message(STATUS " Lunar Configuration Summary")
message(STATUS "------------------------------------")
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
message(STATUS " Vendor Only:        ${LUNAR_VENDOR_ONLY}")
message(STATUS "====================================")
message(STATUS "")