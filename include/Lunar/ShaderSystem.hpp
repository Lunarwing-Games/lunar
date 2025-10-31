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

// Remember, this targets OpenGL 2.0 from April 2004
// Shaders are going to be written using '#version 110' !!

namespace Lunar::Graphics
{

    class Shader
    {
    public:
        enum class Type { Vertex, Fragment };

        /// @brief Shader constructor
        /// @param type What type of shader is being built
        /// @param source Shader source code; (.vert) or (.frag)
        Shader(Type type, const std::string &source);

        ~Shader();

        /// @brief Compiles the shader
        /// @return boolean
        bool Compile();

        /// @brief Constant function to get the shaders ID.
        /// @return `GLuint` of the ID
        GLuint ID() const noexcept { return m_ID; }

    private:
        GLuint m_ID{};
        Type m_Type;
        std::string m_Source;
    };
}