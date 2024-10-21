#include "Terrain.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../stb_image.h"

Terrain::Terrain(int scale, int subdivision, float heightScale, const std::string& texturePath)
    : m_width(scale), m_depth(scale), m_heightScale(heightScale), m_subdivision(subdivision), m_texturePath(texturePath)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    initVertexAndIndices();
    generateMesh();
}

void Terrain::initVertexAndIndices()
{
    float halfWidth = m_width / 2.0f;
    float halfDepth = m_depth / 2.0f;
    float deltaX = m_width / m_subdivision;
    float deltaZ = m_depth / m_subdivision;
    float textureRepeatFactor = 4.0f;

    m_vertices.clear();
    
    for (int z = 0; z <= m_subdivision; ++z)
    {
        for (int x = 0; x <= m_subdivision; ++x)
        {
            float posX = -halfWidth + x * deltaX;
            float posZ = -halfDepth + z * deltaZ;

            float posY = static_cast<float>(std::rand()) / RAND_MAX * m_heightScale - (m_heightScale / 2.0f);

            float texCoordX = static_cast<float>(x) / m_subdivision * textureRepeatFactor;
            float texCoordZ = static_cast<float>(z) / m_subdivision * textureRepeatFactor;

            m_vertices.emplace_back(Vertex{
                {posX, posY, posZ},
                {0.0f, 1.0f, 0.0f},
                {texCoordX, texCoordZ}
            });
        }
    }

    m_indices.clear();
    for (int z = 0; z < m_subdivision; ++z)
    {
        for (int x = 0; x < m_subdivision; ++x)
        {
            int topLeft = z * (m_subdivision + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (m_subdivision + 1) + x;
            int bottomRight = bottomLeft + 1;

            m_indices.push_back(topLeft);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(topRight);

            m_indices.push_back(bottomLeft);
            m_indices.push_back(bottomRight);
            m_indices.push_back(topRight);
        }
    }
}

void Terrain::generateMesh()
{
    if (!m_texturePath.empty())
    {
        Texture texture;
        texture.id = textureFromFile(m_texturePath.c_str(), "");
        texture.type = "texture_diffuse";
        texture.path = m_texturePath;
        m_textures.push_back(texture);
    }
    Mesh terrainMesh(m_vertices, m_indices, m_textures);
    addMesh(terrainMesh);
}

unsigned Terrain::textureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    if (!directory.empty())
        filename = directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
