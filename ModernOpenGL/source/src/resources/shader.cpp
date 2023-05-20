#include "resources/shader.hpp"

#include <fstream>

#include "core/debug/logger.hpp"

Shader::Shader(std::filesystem::path& folder)
{
    folder = std::filesystem::relative(folder);

    LoadVertex(folder.string() + "\\vertex.vs");
    LoadFragment(folder.string() + "\\fragment.fs");

    Link();
}

bool Shader::LoadVertex(const std::filesystem::path& filepath)
{
    Logger::LogInfo("Compiling vertex shader: %s", filepath.string().c_str());
    return LoadShader(filepath, mVertex, ShaderType::Vertex);
}

bool Shader::LoadFragment(const std::filesystem::path& filepath)
{
    Logger::LogInfo("Compiling fragment shader: %s", filepath.string().c_str());
    return LoadShader(filepath, mFragment, ShaderType::Fragment);
}

bool Shader::LoadShader(const std::filesystem::path &filepath, unsigned int& shader, const ShaderType type)
{
    Load(filepath);

    if (!mLoaded)
        return false;
    mLoaded = false;

    GLenum shaderType = 0;
    switch (type)
    {
        case ShaderType::Vertex:
            shaderType = GL_VERTEX_SHADER;
            break;
            
        case ShaderType::Fragment:
            shaderType = GL_FRAGMENT_SHADER;
            break;
    }

    shader = glCreateShader(shaderType);
    const char* const source = mSource.c_str();
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        Logger::LogError("Shader compilation failed: %s", infoLog);
        return false;
    }

    return true;
}

void Shader::Load(const std::filesystem::path &filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open() || !file.good())
    {
        Logger::LogError("Could not open shader file: %s", std::filesystem::absolute(filepath).string().c_str());
        return;
    }

    mSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    mLoaded = true;
}

void Shader::Unload()
{
    mSource = "";
    mLoaded = false;
    
    glDeleteShader(mVertex);
    glDeleteShader(mFragment);
}

bool Shader::Link()
{
    Logger::LogInfo("Linking shader program");

    mProgram = glCreateProgram();
    glAttachShader(mProgram, mVertex);
    glAttachShader(mProgram, mFragment);
    glLinkProgram(mProgram);

    int success;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(mProgram, 512, NULL, infoLog);
        Logger::LogError("Shader program linking failed: %s", infoLog);
        return false;
    }

    Unload();

    return true;
}
