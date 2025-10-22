#pragma once
// #include "lua.h"
// #include "lualib.h"
// #include "lauxlib.h"
#include <string>
#include <vector>
#include <cstdio>

namespace Lunar::Lua
{
    enum class LInitFlags : uint32_t
    {
        None         = 0,       /// Bind nothing to Lua's VM
        BindECS      = 1 << 0,  /// Flag to bind the ECS module to Lua
        BindRender2D = 1 << 1,  /// Flag to bind the 2D renderer to Lua
        BindRender3D = 1 << 2,  /// Flag to bind the 3D renderer to Lua
        BindLogging  = 1 << 3,  /// Flag to bind the logger (external) to Lua
    };

    inline LInitFlags operator|(LInitFlags a, LInitFlags b)
    {
        return static_cast<LInitFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline bool operator&(LInitFlags a, LInitFlags b)
    {
        return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
    }

    /// @brief Initalizes a Lua sandbox/VM environment.
    /// @param flags 
    /// @return `bool`
    bool Init(LInitFlags flags = LInitFlags::None);

    /// @brief Shuts down the Lua sandbox/VM safely.
    void Shutdown();

    /// @brief Executes a Lua script within the sandbox.
    /// @param filepath Path to the Lua script
    /// @return True on success, false on failure.
    bool RunFile(const std::string &filepath);

    /// @brief Executes Lua code immediately.
    /// @param code The Lua source code to exec.
    /// @return True on LUA_OK, false on error
    bool RunString(const std::string &code);

    /// @brief Gets the current state of Lua
    /// @return A pointer to the `lua_State`
    void GetState() noexcept;

    /// @brief Reset the Lua VM 
    void ResetState();

    /// @brief **Internal** function to bind the ECS module to Lua if req'd by the user.
    void __BindECS();

    /// @brief  **Internal** function to bind the 2D render module if req'd by the user.
    void __BindRender2D();

    /// @brief  **Internal** function to bind the 3D render module if req'd by the user.
    void __BindRender3D();

    /// @brief **Internal** function to bind logging to Lua if req'd by the user.
    void __BindLogging();
}