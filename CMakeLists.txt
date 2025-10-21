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
option(LUNAR_BUILD_WORKERS "Sets the amount of threads to use for compilation." 1)
option(LUNAR_VENDOR_ONLY "Disables FetchContent and relies on local dependencies." ON)

if(LUNAR_BUILD_WORKERS GREATER 1)
    set(CMAKE_BUILD_PARALELL_LEVEL ${LUNAR_BUILD_WORKERS})
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

#
# Fetching dependencies (if allowed)
# 

include(FetchContent)

macro(lunar_fetch name repo tag)
    message(STATUS "Fetching ${name} ...")
    FetchContent_Declare(${name}
        GIT_REPOSITORY ${repo}
        GIT_TAG        ${tag}
    )
    FetchContent_MakeAvailable(${name})
endmacro()

#
# Files, conditional modules
#

file(GLOB_RECURSE LUNAR_SRC CONFIGURE_DEPENDS "src/*.cpp")
add_library(Lunar STATIC ${LUNAR_SRC})

target_include_directories(Lunar PUBLIC include thirdparty)

if(LUNAR_SCRIPT_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_ENABLE_SCRIPT)
endif()
if(LUNAR_LOGGING_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_ENABLE_LOG)
endif()
if(LUNAR_2DREND_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_ENABLE_2D)
endif()
if(LUNAR_3DREND_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_ENABLE_3D)
endif()
if(LUNAR_ECS_MODULE)
    target_compile_definitions(Lunar PUBLIC LUNAR_ENABLE_ECS)
endif()
