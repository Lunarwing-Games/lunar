#pragma once
#include <string>

#if defined(_WIN32) || defined(_WIN64)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>
#endif

#include "LogSystem.hpp"
#include "glad/gl.h"
#include "SDL3/SDL.h"

namespace Lunar::Graphics
{

    class Shader
    {
    public:
        enum class Type { Vertex, Fragment };

        Shader(Type type, const std::string &source);
        ~Shader();

        bool Compile();
        GLuint ID() const noexcept { return m_ID; }

    private:
        GLuint m_ID{};
        Type m_Type;
        std::string m_Source;
    };
}