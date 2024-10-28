#pragma once
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
    void setLight(const std::string &colorName, glm::vec3 colorValue,const std::string &ambientName, float ambientValue ) const;
    void setDirectionalLight(const std::string &ambientColorName, glm::vec3 ambientColorValue,const std::string &ambientName, float ambientValue,const std::string &diffuseName, float diffuseValue,const std::string &directionName, glm::vec3 directionValue ) const;
    void setMaterial(const std::string &name, Material& value) const;
};  
