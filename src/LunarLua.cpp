#include "Lunar/LunarLua.hpp"

namespace Lunar::Lua
{
    static lua_State *L = nullptr;

    bool Init(LInitFlags flags)
    {
        if (L) return false;    // VM already exists
        L = luaL_newstate();
        if (!L) return false;   // VM creation error

        // Specify libs to open up (this is to avoid security nightmares!)
        luaL_requiref(L, "_G", luaopen_base, 1);
        luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
        luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
        luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
        luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);
        lua_pop(L, 5);

        // Flag checking and binding as needed
        if (flags & LInitFlags::BindECS) __BindECS();
        if (flags & LInitFlags::BindRender2D) __BindRender2D();
        if (flags & LInitFlags::BindRender3D) __BindRender3D();
        if (flags & LInitFlags::BindLogging) __BindLogging();
    }

    void Shutdown() {
    if (!L) return;
    lua_close(L);
    L = nullptr;
    }

    bool RunFile(const std::string &filepath)
    {
        if (!L) return false;
        return luaL_dofile(L, filepath.c_str()) == LUA_OK;
    }

    lua_State *GetState() noexcept { return L; }

    void ResetState() {
        Shutdown();
        Init();
    }

    void __BindECS() { /* placeholder */ }
    void __BindRender2D() { /* placeholder */ }
    void __BindRender3D() { /* placeholder */ }
    void __BindLogging() { /* placeholder */ }
}