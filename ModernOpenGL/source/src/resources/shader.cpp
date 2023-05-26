#include "resources/shader.hpp"

#include <fstream>

#include "core/debug/logger.hpp"

Shader::Shader(std::filesystem::path& folder, const std::string& vertexName, const std::string& fragmentName)
{
    folder = std::filesystem::relative(folder);

    if (!LoadVertex(folder.string() + '\\' + vertexName + ".vs"))
        return;
    if (!LoadFragment(folder.string() + '\\' + fragmentName + ".fs"))
        return;

    Link();
}

bool Shader::LoadVertex(const std::filesystem::path& filepath)
{
    if (mVertex != 0)
        Unload(mVertex);

    Logger::LogInfo("Compiling vertex shader: %s", filepath.string().c_str());
    return LoadShader(filepath, mVertex, ShaderType::Vertex);
}

bool Shader::LoadFragment(const std::filesystem::path& filepath)
{
    if (mFragment != 0)
        Unload(mFragment);

    Logger::LogInfo("Compiling fragment shader: %s", filepath.string().c_str());
    return LoadShader(filepath, mFragment, ShaderType::Fragment);
}

bool Shader::LoadShader(const std::filesystem::path &filepath, unsigned int& shader, const ShaderType type)
{
    if (!Load(filepath))
        return false;

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

bool Shader::Load(const std::filesystem::path &filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open() || !file.good())
    {
        Logger::LogError("Could not open shader file: %s", std::filesystem::absolute(filepath).string().c_str());
        return false;
    }

    mSource = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return true;
}

void Shader::Unload()
{
    mSource = "";
    
    Unload(mVertex);
    Unload(mFragment);
}

void Shader::Unload(unsigned int& shader)
{
    glDeleteShader(shader);
    shader = 0;
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
