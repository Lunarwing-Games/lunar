#include "Lunar/ShaderSystem.hpp"
#include "Lunar/ShaderProgram.hpp"
#include "Lunar/LogSystem.hpp"
#include "glad/gl.h"
#include <format>

namespace Lunar::Graphics
{
    ShaderProgram::ShaderProgram()
    {
        m_ID = glCreateProgram();
        if (!m_ID)
        {
            Logger<ShaderProgram>::Error("Failed to create shader program!");
        }
    }

    ShaderProgram::~ShaderProgram()
    {
        if (m_ID != 0)
        {
            glDeleteProgram(m_ID);
            m_ID = 0;
        }
    }

    bool ShaderProgram::Attach(const Shader &shader)
    {
        if (!shader.ID())
        {
            Logger<ShaderProgram>::Warn("Tried to attach invalid shader.");
            return false;
        }

        glAttachShader(m_ID, shader.ID());
        return true;
    }

    bool ShaderProgram::Link()
    {
        glLinkProgram(m_ID);

        GLint success = 0;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint logLen = 0;
            glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &logLen);

            std::string log(logLen, '\0');
            glGetProgramInfoLog(m_ID, logLen, nullptr, log.data());

            Logger<ShaderProgram>::Error(std::format("Program linking failed: {}", log));
            return false;
        }

        Logger<ShaderProgram>::Info(std::format("Shader program linked successfully (ID = {})", m_ID));
        return true;
    }

    void ShaderProgram::Use() const noexcept
    {
        glUseProgram(m_ID);
    }

    GLint ShaderProgram::GetUniformLocation(const std::string &name)
    {
        if (auto it = m_UniformCache.find(name); it != m_UniformCache.end())
            return it->second;

        GLint loc = glGetUniformLocation(m_ID, name.c_str());
        if (loc == -1)
            Logger<ShaderProgram>::Warn(std::format("Uniform '{}' not found in program.", name));

        m_UniformCache[name] = loc;
        return loc;
    }
}