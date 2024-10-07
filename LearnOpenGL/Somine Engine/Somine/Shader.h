#pragma once
#include <string>
#include <sstream>
#include <glm/glm/detail/type_mat.hpp>


class Shader
{
public:
    unsigned int id;
    
    Shader(); 
    Shader(const char* vertex_path, const char* fragment_path);

    void use() const;
    void setBool(const std::string &name, bool value) const; 
    void setInt(const std::string &name, int value) const; 
    void setFloat(const std::string &name, float value) const; 
    void setUniformMatrix4fv(const std::string &name, glm::mat4 value) const; 
};
