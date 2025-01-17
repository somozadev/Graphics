﻿#pragma once
#include <string>
#include <sstream>
#include <glm/glm/detail/type_mat.hpp>
#include <glm/glm/detail/type_vec.hpp>

#include "assimpLoader/Material.h"


class Shader
{
public:
    unsigned int id;
    
    Shader();
    ~Shader();
    
    Shader(const char* vertex_path, const char* fragment_path);

    void use() const;
    void setBool(const std::string &name, bool value) const; 
    void setInt(const std::string &name, int value) const; 
    void setFloat(const std::string &name, float value) const; 
    void setUniformMatrix4fv(const std::string &name, glm::mat4 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setLight(const std::string &colorName, glm::vec3 colorValue,const std::string &ambientName, float ambientValue ) const;
    void setDirectionalLight(const std::string &ambientColorName, glm::vec3 ambientColorValue,const std::string &ambientName, float ambientValue,const std::string &diffuseName, float diffuseValue,const std::string &directionName, glm::vec3 directionValue ) const;
    void setPointLight(int index, glm::vec3 color, float ambient_intensity, float diffuse_intensity, glm::vec3 local_pos, float constant_att, float linear_att, float exponential_att) const;
    void setSpotLight(int index, glm::vec3 color, float ambient_intensity, float diffuse_intensity, glm::vec3 local_pos, float constant_att, float linear_att, float exponential_att,  glm::vec3 local_direction, float cutoff) const;
    void setMaterial(const std::string &name, Material& value) const;
};  
