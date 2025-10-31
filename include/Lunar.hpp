#pragma once

/** Welcome to the main header for Lunar!
 * 
 * If you're looking for complete documentation...you can build them yourself 
 * if you have doxygen installed and ready on your system. If this is a
 * pre-built release of Lunar, a pre-made set of docs can be found in the
 * provided "DOCS" directory.
 * 
 * If you are using the latest commit and have internet access, docs are
 * easily accessible at the organizations website!
 * 
 * permalink -> https://lunarwing-games.github.io
 * 
 * NOTE: You do NOT need to define any macros to use Lunar. These macros
 *       are defined at config time with the provided CMakeLists.txt.
 */

#ifndef __cplusplus
    #error "Lunar MUST be compiled with a C++ compiler!"
#endif

#if __cplusplus < 202002L || !(defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    #error "Lunar requires C++20 at the minimum!"
#endif

// The Diag module is ALWAYS included, you can remove it at your own risk.
// It provides important multiplatform diagnostic features for Lunar.
#include "Lunar/Diagnostics.hpp"

#ifdef LUNAR_2DRENDER_ENABLED
    #include "Lunar/2DRenderer.hpp"
#endif

#ifdef LUNAR_3DRENDER_ENABLED
    #include "Lunar/3DRenderer.hpp"
#endif

#ifdef LUNAR_ECS_ENABLED
    #include "Lunar/EntitySystem.hpp"
#endif

#ifdef LUNAR_LOG_ENABLED
    #include "Lunar/LogSystem.hpp"
#endif

#ifdef LUNAR_LUA_ENABLED
    #include "Lunar/LuaSystem.hpp"
#endif

// Coming soon!
#ifdef LUNAR_UI_ENABLED
    #include "Lunar/UISystem.hpp"
#endif

#ifdef LUNAR_SHADERS_ENABLED
    #include "Lunar/ShaderSystem.hpp"
    #include "Lunar/ShaderProgram.hpp"
#endif