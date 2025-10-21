#include "Lunar/LunarLua.hpp"

namespace Lunar::Lua
{
    static lua_State *L = nullptr;
    static LInitFlags s_lastFlags = LInitFlags::None;

    bool Init(LInitFlags flags)
    {
        if (L) return false;    // VM already exists
        s_lastFlags = flags;

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
        
        return true;
    }

    void Shutdown() {
        if (!L) return;
        lua_close(L);
        L = nullptr;
    }

    bool RunFile(const std::string &filepath)
    {
        if (!L) return false;
        
        
        int status = luaL_dofile(L, filepath.c_str());
        if (status != LUA_OK)
        {
            const char *err = lua_tostring(L, -1);
            // fprintf logging is temp until we get the logging module working
            fprintf(stderr, "[LUA] Error running file '%s': %s\n",
                    filepath.c_str(), err ? err : "(undefined)");
            lua_pop(L, 1);
            return false;
        }
        return true;
    }

    bool RunString(const std::string &code)
    {
        if (!L) return false;

        int status = luaL_dostring(L, code.c_str());
        if (status != LUA_OK)
        {
            const char *err = lua_tostring(L, -1);
            // fprintf logging is temp until we get the logging module working
            fprintf(stderr, "[LUA] Error executing code: %s\n",
                    err ? err : "(undefined)");
            lua_pop(L, 1);
            return false;
        }
        return true;
    }

    lua_State *GetState() noexcept { return L; }

    void ResetState() {
        Shutdown();
        Init(s_lastFlags);
    }

    void __BindECS() { /* placeholder */ }
    void __BindRender2D() { /* placeholder */ }
    void __BindRender3D() { /* placeholder */ }
    void __BindLogging() { /* placeholder */ }
}