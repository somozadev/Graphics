#pragma once
#include <string>
#include <glm/glm/detail/type_vec.hpp>
#include <glm/glm/detail/type_vec3.hpp>

struct Material
{
    // std::string diffuse_texture_filename; 
    // std::string specular_texture_filename; 

    glm::vec3 ambient_color {1.0f, 1.0f, 1.0f}; 
    glm::vec3 diffuse_color {1.0f, 1.0f, 1.0f}; 
    glm::vec3 specular_color {0}; 

    glm::vec3 emissive_color {0};
    // float specular_factor {1.0f}; 
};
