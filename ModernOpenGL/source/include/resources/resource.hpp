#pragma once

#include <filesystem>

class Resource
{
public:
    Resource(const std::filesystem::path& filepath) { Load(filepath); }
    virtual ~Resource() { Unload(); }

    virtual void Load(const std::filesystem::path& filepath) = 0;
    virtual bool IsLoaded() const { return !mFilepath.empty(); }
    virtual void Unload() = 0;

protected:
    std::filesystem::path mFilepath;
};
