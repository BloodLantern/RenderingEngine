#include "resources/model.hpp"

#include <fstream>

#include <glad/glad.h>

#include "core/debug/logger.hpp"

void Model::Load(const std::filesystem::path& filepath)
{
    Logger::LogInfo("Loading model: %s", filepath.string().c_str());

    std::ifstream file(filepath);

    if (!file.is_open() || !file.good())
    {
        Logger::LogError("Failed to open file at path: %s", filepath.string().c_str());
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
                // OpenGL bad
                uv.y = 1.f - uv.y;
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
        if (line[0] != 'f')
            continue;

        unsigned int modelIndices[3][3];
        sscanf_s(
            line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
            &modelIndices[0][0], &modelIndices[0][1], &modelIndices[0][2],
            &modelIndices[1][0], &modelIndices[1][1], &modelIndices[1][2],
            &modelIndices[2][0], &modelIndices[2][1], &modelIndices[2][2]
        );

        mVertices.push_back(Vertex(positions[modelIndices[0][0] - 1], uvs[modelIndices[0][1] - 1], normals[modelIndices[0][2] - 1]));
        mVertices.push_back(Vertex(positions[modelIndices[1][0] - 1], uvs[modelIndices[1][1] - 1], normals[modelIndices[1][2] - 1]));
        mVertices.push_back(Vertex(positions[modelIndices[2][0] - 1], uvs[modelIndices[2][1] - 1], normals[modelIndices[2][2] - 1]));

        for (unsigned int i = 0; i < 9; i++)
            mIndices.push_back(modelIndices[i / 3][i % 3]);
    }
}

void Model::Unload()
{
    mVertices.clear();
    mIndices.clear();

    if (mLinked)
    {
        glDeleteVertexArrays(1, &mVAO);
        glDeleteBuffers(2, &mVBO);

        mLinked = false;
    }
}

void Model::Draw()
{
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, (int) mVertices.size());
}

bool Model::Link()
{
    if (mLinked)
        return false;

    Logger::LogInfo("Linking model");

    // Generate the VAO
    glGenVertexArrays(1, &mVAO);
    // Generate the necessary buffers and set VBO and EBO ids
    glGenBuffers(2, &mVBO);

    // Setup the VAO
    glBindVertexArray(mVAO);

    // Setup the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    // Setup the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*) sizeof(Vector3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*) (sizeof(Vector3) + sizeof(Vector2)));
    glEnableVertexAttribArray(2);

    // Unbind the VAO
    glBindVertexArray(0);
    // And the buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return mLinked = true;
}
