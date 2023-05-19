#pragma once

#include <vector>

#include "resources/resource.hpp"
#include "core/data_structure/vertex.hpp"

class Model : public Resource
{
public:
    Model(const std::string& filepath) : Resource(filepath) {}

	// Inherited via Resource
	virtual void Load(const std::filesystem::path& filepath) override;
	virtual void Unload() override;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
