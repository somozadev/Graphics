#pragma once
#include <glm/glm/detail/type_vec.hpp>
#include <glm/glm/detail/type_vec2.hpp>
#include <glm/glm/detail/type_vec3.hpp>
#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent; // no clue what's this for yet
    glm::vec3 bit_tangent; // no clue what's this for yet

    int m_boneIDs[MAX_BONE_INFLUENCE];
    int m_weights[MAX_BONE_INFLUENCE];
};
