#include "Mesh.h"

#include <utility>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures)
{
    m_vertices = vertices;
    m_indices = indices;
    m_textures = textures;
    setupMesh();
}

void Mesh::createWhiteTexture()
{
    glGenTextures(1, &m_white_texture);
    glBindTexture(GL_TEXTURE_2D, m_white_texture);

    unsigned char whitePixel[3] = {255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whitePixel);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::draw(const Shader* shader)
{
    glBindVertexArray(0);

    GLuint diffuseN = 1;
    GLuint specularN = 1;
    GLuint normalN = 1;
    GLuint heightN = 1;

    bool hasDiffuse = false;
    for (const auto& texture : m_textures)
    {
        if (texture.type == "texture_diffuse")
        {
            hasDiffuse = true;
            break;
        }
    }
    if (!hasDiffuse) {
        glActiveTexture(GL_TEXTURE0); 
        shader->setInt("texture_diffuse1", 0); 
        glBindTexture(GL_TEXTURE_2D, m_white_texture); 
    }
    for (GLuint i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = m_textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseN++);
        else if (name == "texture_specular")
            number = std::to_string(specularN++);
        else if (name == "texture_normal")
            number = std::to_string(normalN++);
        else if (name == "texture_height")
            number = std::to_string(heightN++);

        shader->setInt(name + number, i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (GLuint i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);


    //right now i have 1 VAO and 1 VBO per mesh, we can change it to be
    // 1 VAO and / or 1 VBO per model, having inside them multiple meshes
    //it might be a hardware limitation decission, pretty much based on which platform you are targetting 


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));


    glBindVertexArray(0);
}
