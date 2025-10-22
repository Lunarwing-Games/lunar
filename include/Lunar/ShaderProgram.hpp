#pragma once
#include "ShaderSystem.hpp"
#include <unordered_map>
#include <string>

namespace Lunar::Graphics
{
    class ShaderProgram
    {
    public:
        ShaderProgram();
        ~ShaderProgram();

        bool Attach(const Shader &shader);
        bool Link();
        void Use() const noexcept;

        GLint GetUniformLocation(const std::string &name);
    
    private:
        GLuint m_ID{};
        std::unordered_map<std::string, GLint> m_UniformCache;
    };
}