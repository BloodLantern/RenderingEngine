#include "resources/texture.hpp"

#include <stb_image.h>

#include <vector2i.hpp>

#include "core/debug/logger.hpp"

void Texture::Load(const std::filesystem::path& filepath)
{
    Logger::LogInfo("Loading texture: %s", filepath.string().c_str());

    int nbrChannels;
    Vector2i size;
    unsigned char* data = stbi_load(filepath.string().c_str(), &size.x, &size.y, &nbrChannels, 0);
    if (!data)
    {
        Logger::LogError("Failed to load texture: %s", filepath.string().c_str());
        return;
    }

    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, nbrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    mLoaded = true;
}
