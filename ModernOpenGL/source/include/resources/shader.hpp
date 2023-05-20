#pragma once

#include "resources/resource.hpp"

#include <string>

#include <glad/glad.h>

class Shader : public Resource
{
public:
    enum class ShaderType : unsigned char
    {
        Vertex,
        Fragment
    };

    Shader() = default;
    Shader(std::filesystem::path& folder);
    Shader(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile) { LoadVertex(vertexFile); LoadFragment(fragmentFile); }
    ~Shader() { glDeleteProgram(mProgram); }

    bool LoadVertex(const std::filesystem::path& filepath);
    bool LoadFragment(const std::filesystem::path& filepath);
    bool Link() override;

    void Use() const { glUseProgram(mProgram); }

private:
    unsigned int mVertex = 0, mFragment = 0, mProgram = 0;
    std::string mSource;

    bool LoadShader(const std::filesystem::path& filepath, unsigned int& shader, const ShaderType type);

    // Load shader source
    void Load(const std::filesystem::path& filepath) override;
    void Unload() override;
};
