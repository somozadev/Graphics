#include "Mesh.h"

Mesh::Mesh()
{
    m_model_matrix = glm::mat4(m_scale);
}


void Mesh::generateBuffers()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * getVertices().size(), getVertices().data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * getIndices().size(), getIndices().data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_indicesSize = m_indices.size();
    glBindVertexArray(0);
}

std::vector<GLfloat> Mesh::getVertices()
{
    return m_vertices;
}

std::vector<GLuint> Mesh::getIndices()
{
    return m_indices;
}

GLuint64 Mesh::getIndicesSize()
{
    return m_indicesSize;
}

glm::mat4 Mesh::getModelMatrix()
{
    return m_model_matrix;
}

Mesh::~Mesh()
{
    m_vertices.clear();
    m_indices.clear();
    m_textures.clear();

    glDeleteBuffers(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}
