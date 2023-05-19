#include "resources/model.hpp"

#include <fstream>

#include "core/debug/logger.hpp"

void Model::Load(const std::filesystem::path& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open() || !file.good())
    {
        Logger::LogError("Failed to open file at path: %s", filepath.string());
        return;
    }

    std::vector<Vector3> positions, normals;
    std::vector<Vector2> uvs;

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        if (line.empty() || line[0] == '#')
            continue;

        if (line[0] == 'v')
        {
                if (line[1] == 't')
                {
                    Vector2 uv;
                    sscanf_s(line.c_str(), "vt %f %f", &uv.x, &uv.y);
                    uvs.push_back(uv);
                }
                else if (line[1] == 'n')
                {
                    Vector3 normal;
                    sscanf_s(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                    normals.push_back(normal);
                }
                else
                {
                    Vector3 position;
                    sscanf_s(line.c_str(), "v %f %f %f", &position.x, &position.y, &position.z);
                    positions.push_back(position);
                }
                continue;
        }

        // Currently doesn't support materials
        if (!line[0] == 'f')
            continue;

        unsigned int modelIndices[3][3];
        sscanf_s(
            line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
            &modelIndices[0][0], &modelIndices[0][1], &modelIndices[0][2],
            &modelIndices[1][0], &modelIndices[1][1], &modelIndices[1][2],
            &modelIndices[2][0], &modelIndices[2][1], &modelIndices[2][2]
        );

        vertices.push_back(Vertex(positions[modelIndices[0][0] - 1], normals[modelIndices[0][1] - 1], uvs[modelIndices[0][2] - 1]));
        vertices.push_back(Vertex(positions[modelIndices[1][0] - 1], normals[modelIndices[1][1] - 1], uvs[modelIndices[1][2] - 1]));
        vertices.push_back(Vertex(positions[modelIndices[2][0] - 1], normals[modelIndices[2][1] - 1], uvs[modelIndices[2][2] - 1]));

        for (unsigned int i = 0; i < 9; i++)
            indices.push_back(modelIndices[i / 3][i % 3]);
    }
}

void Model::Unload()
{
}
