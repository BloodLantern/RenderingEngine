#pragma once

#include <vector>

#include "resources/resource.hpp"
#include "core/data_structure/vertex.hpp"

class Model : public Resource
{
public:
    Model(const std::string& filepath) : Resource(filepath) {}
    virtual ~Model() { Unload(); }

	void Load(const std::filesystem::path& filepath) override;
	void Unload() override;

private:
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    unsigned int mVBO = 0, mEBO = 0, mVAO = 0;

    bool Link() override;
};
