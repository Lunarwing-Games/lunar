# Configuring Lunar
<!-- I can't wait for this project to balloon in complexity! -->
Lunar uses **CMake** as its build system. This document outlines the common configuration options and environment variables for building the framework.

## Prerequisites

Before configuring the build, ensure you have the following installed and ready:
 - **CMake** $$\geq$$ 3.20
 - A **C++20** compatible compiler
    * GCC $$\geq$$ 8
    * Clang partially supports C++20
    * MSVC $$\geq$$ 16.11

## Configuring and Building

To build a generic build with all the baseline features, just run this command in the root of the repository;

```bash
cmake -S . -B (build_dir) -G (generator)
```

Lunar supports most of the basic build types;

| Type | Description |
| ---- | ----------- |
| Debug | No optimizations, debug symbols. |
| Release | Very optimized, stripped of symbols. |
| MinSizeRel | Size-optimized library with release optimization. |

The build type of `RelWithDebInfo` is specifically not supported by this project.

### Common CMake Options

| Options | Default | Description |
| ------- | ------- | ----------- |
| `LUNAR_INTERNAL_LOGGING` | `OFF` | Framework internal logging subsystem. |
| `LUNAR_SCRIPT_MODULE` | `ON` | Lua module. |
| `LUNAR_LOGGING_MODULE` | `ON` | Logging for the users of Lunar. |
| `LUNAR_2DREND_MODULE` | `ON` | 2D Rendering module. |
| `LUNAR_3DREND_MODULE` | `ON` | 3D Rendering module. |
| `LUNAR_ECS_MODULE` | `ON` | Entity-Component-System module. |
| `LUNAR_PEDANTIC_COMP` | `OFF` | Highest level of compilation warnings. |
| `LUNAR_ERROR_ON_WARN` | `OFF` | Treats warnings as errors. `-Werror` or `/WX` |
| `LUNAR_BUILD_TESTS` | `OFF` | Builds the framework and a suite of tests. |
| `LUNAR_BUILD_WORKERS` | `1` | Amount of threads/jobs to use for building. |
