#include "OldMesh.h"

#include <glm/glm/gtc/matrix_transform.hpp>

OldMesh::OldMesh()
{
    m_model_matrix = glm::mat4(m_scale);
}


void OldMesh::generateBuffers()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * getVertices().size(), getVertices().data(),
                 GL_STATIC_DRAW);

    if (getIndices().size() > 0)
    {
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * getIndices().size(), getIndices().data(),
                     GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_indicesSize = m_indices.size();
    glBindVertexArray(0);
}

std::vector<GLfloat> OldMesh::getVertices()
{
    return m_vertices;
}

std::vector<GLuint> OldMesh::getIndices()
{
    return m_indices;
}

GLuint64 OldMesh::getIndicesSize()
{
    return m_indicesSize;
}

glm::mat4 OldMesh::getModelMatrix()
{
    return m_model_matrix;
}

void OldMesh::moveTo(float x, float y, float z)
{
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, glm::vec3(x, y, z));
}

auto OldMesh::rotate(float angle, glm::vec3 axis) -> void
{
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(angle), axis);
}

OldMesh::~OldMesh()
{
    m_vertices.clear();
    m_indices.clear();
    m_textures.clear();

    glDeleteBuffers(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}
