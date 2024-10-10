#pragma once
#include <string>
#include <glm/glm/detail/type_vec.hpp>
#include <glm/glm/detail/type_vec4.hpp>

struct Material
{
    std::string diffuse_texture_filename; 
    std::string specular_texture_filename; 

    glm::vec4 difusse_color {1}; 
    glm::vec4 specular_color {1}; 
    glm::vec4 ambient_color {1}; 
    glm::vec4 emissive_color {0};
    float specular_factor {1.0f}; 

};
