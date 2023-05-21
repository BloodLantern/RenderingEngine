#pragma once

#include "resources/resource.hpp"

#include <string>

#include <glad/glad.h>

#include <vector2.hpp>
#include <vector3.hpp>
#include <vector4.hpp>
#include <matrix2x2.hpp>
#include <matrix3x3.hpp>
#include <matrix4x4.hpp>

class Shader : public Resource
{
public:
    enum class ShaderType : unsigned char
    {
        Vertex,
        Fragment
    };

    Shader() = default;
    Shader(std::filesystem::path& folder, const std::string& vertexName = "vertex", const std::string& fragmentName = "fragment");
    Shader(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile) { LoadVertex(vertexFile); LoadFragment(fragmentFile); }
    ~Shader() { glDeleteProgram(mProgram); }

    bool LoadVertex(const std::filesystem::path& filepath);
    bool LoadFragment(const std::filesystem::path& filepath);
    bool Link();

    void Use() const { glUseProgram(mProgram); }

    inline unsigned int GetUniform(const std::string& name) const { return glGetUniformLocation(mProgram, name.c_str()); }
    inline void SetUniform(const std::string& name, const bool value) const { SetUniform(name, (int) value); }
    inline void SetUniform(const std::string& name, const int value) const { glUniform1i(GetUniform(name), value); }
    inline void SetUniform(const std::string& name, const float value) const { glUniform1f(GetUniform(name), value); }
    inline void SetUniform(const std::string& name, const Vector2 value) const { glUniform2fv(GetUniform(name), 1, &value.x); }
    inline void SetUniform(const std::string& name, const Vector3& value) const { glUniform3fv(GetUniform(name), 1, &value.x); }
    inline void SetUniform(const std::string& name, const Vector4& value) const { glUniform4fv(GetUniform(name), 1, &value.x); }
    inline void SetUniform(const std::string& name, const Matrix2x2& value) const { glUniformMatrix2fv(GetUniform(name), 1, true, &value[0].x); }
    inline void SetUniform(const std::string& name, const Matrix3x3& value) const { glUniformMatrix3fv(GetUniform(name), 1, true, &value[0].x); }
    inline void SetUniform(const std::string& name, const Matrix4x4& value) const { glUniformMatrix4fv(GetUniform(name), 1, true, &value[0].x); }

private:
    unsigned int mVertex = 0, mFragment = 0, mProgram = 0;
    std::string mSource;
    bool mLinked = false;

    bool LoadShader(const std::filesystem::path& filepath, unsigned int& shader, const ShaderType type);

    // Load shader source
    void Load(const std::filesystem::path& filepath) override;
    void Unload() override;
    void Unload(unsigned int& shader);
};
