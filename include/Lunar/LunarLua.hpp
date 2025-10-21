#pragma once
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <string>
#include <vector>

namespace Lunar::Lua
{
    enum class LInitFlags : uint32_t
    {
        None         = 0,
        BindECS      = 1 << 0,
        BindRender2D = 1 << 1,
        BindRender3D = 1 << 2,
        BindLogging  = 1 << 3,
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

    /// @brief 
    /// @return 
    lua_State *GetState() noexcept;

    /// @brief 
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