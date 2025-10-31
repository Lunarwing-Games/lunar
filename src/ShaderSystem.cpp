#include "Lunar/ShaderSystem.hpp"
#include "glad/gl.h"
#include <vector>
#include <format>

namespace Lunar::Graphics
{

    /// @brief Global helper to convert Shader::Type to GL shader enum
    static GLenum ToGLShaderType(Shader::Type type)
    {
        switch (type)
        {
            case Shader::Type::Vertex:   return GL_VERTEX_SHADER;
            case Shader::Type::Fragment: return GL_FRAGMENT_SHADER;
            default:                     return 0;
        }
    }

    Shader::Shader(Type type, const std::string &source)
        : m_Type(type), m_Source(source)
    {
        // GLenum glType = (type == Type::Vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
        GLenum glType = ToGLShaderType(type);
        m_ID = glCreateShader(glType);

        if (!m_ID)
            Logger<Shader>::Error("Failed to create a shader object!");
    }

    Shader::~Shader()
    {
        if (m_ID != 0)
        {
            glDeleteShader(m_ID);
            m_ID = 0;
        }
    }

    bool Shader::Compile()
    {
        const char *src = m_Source.c_str();
        glShaderSource(m_ID, 1, &src, nullptr);
        glCompileShader(m_ID);

        GLint success = 0;
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint logLen = 0;
            glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &logLen);

            std::string log(logLen, '\0');
            glGetShaderInfoLog(m_ID, logLen, nullptr, log.data());

            Logger<Shader>::Error(std::format("Shader compilation failed: {}", log));
            return false;
        }

        Logger<Shader>::Info(std::format("Shader compiled OK! (ID = {})", m_ID));
        return true;
    }
}