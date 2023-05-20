#pragma once

#include <filesystem>

class Resource
{
public:
    Resource() {}
    Resource(const std::filesystem::path&) {}
    virtual ~Resource() {}

    virtual void Load(const std::filesystem::path& filepath) = 0;
    virtual bool IsLoaded() const { return mLoaded; }
    virtual void Unload() = 0;

protected:
    bool mLoaded = false, mLinked = false;

    virtual bool Link() = 0;
};
