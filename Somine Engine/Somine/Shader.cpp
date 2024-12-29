#define SHADER_H
#include "Shader.h"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <GLFW/glfw3.h>
#include <glm/glm/detail/type_mat.hpp>
#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/gtc/type_ptr.inl>

#include "assimpLoader/Material.h"
#include "Lights/PointLight.h"
#include <cmath>
#include <glm/glm/detail/func_trigonometric.hpp>


Shader::Shader()
{
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    std::string vertex_code; 
    std::string fragment_code;
    std::ifstream v_shader_file; 
    std::ifstream f_shader_file;

    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream v_shader_stream, f_shader_stream;

        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        v_shader_file.close();
        f_shader_file.close();

        vertex_code = v_shader_stream.str();
        fragment_code= f_shader_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_READ" << std::endl;
    }
    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();



    GLuint vertex {glCreateShader(GL_VERTEX_SHADER)};
    GLuint fragment {glCreateShader(GL_FRAGMENT_SHADER)};
    int success;
    char log_info[512];

    glShaderSource(vertex, 1, &v_shader_code, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, log_info);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log_info << std::endl;
    }

    glShaderSource(fragment, 1, &f_shader_code, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, log_info);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log_info << std::endl;
    }


    id = glCreateProgram();
    glAttachShader(id, vertex); 
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, log_info);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log_info << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
    glUseProgram(id);
}
//here instead of this i can create a UBO to store all this uniform into 4bytes chunks. 
void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
}
void Shader::setUniformMatrix4fv(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec2(const std::string &name, glm::vec2 value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setVec3(const std::string &name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), GL_FALSE, glm::value_ptr(value));
}
void Shader::setLight(const std::string &colorName, glm::vec3 colorValue,const std::string &ambientName, float ambientValue ) const
{
    glUniform3f(glGetUniformLocation(id, colorName.c_str()), colorValue.x, colorValue.y, colorValue.z);
    glUniform1f(glGetUniformLocation(id, ambientName.c_str()), ambientValue);
}
void Shader::setDirectionalLight(const std::string &ambientColorName, glm::vec3 ambientColorValue,const std::string &ambientName, float ambientValue,const std::string &diffuseName, float diffuseValue,const std::string &directionName, glm::vec3 directionValue ) const
{
    glUniform3f(glGetUniformLocation(id, ambientColorName.c_str()), ambientColorValue.x, ambientColorValue.y, ambientColorValue.z);
    glUniform1f(glGetUniformLocation(id, ambientName.c_str()), ambientValue);
    glUniform1f(glGetUniformLocation(id, diffuseName.c_str()), diffuseValue);
    //should use local direction here 
    glUniform3f(glGetUniformLocation(id, directionName.c_str()), directionValue.x, directionValue.y, directionValue.z);
}
void Shader::setPointLight(int index, glm::vec3 color, float ambient_intensity, float diffuse_intensity, glm::vec3 local_pos, float constant_att, float linear_att, float exponential_att) const
{
    
    std::string uniformName = "point_lights[" + std::to_string(index) + "].";
    glUniform3f(glGetUniformLocation(id, (uniformName+"base.color").c_str()), color.r,color.g,color.b);
    glUniform1f(glGetUniformLocation(id, (uniformName+"base.ambient_intensity").c_str()), ambient_intensity);
    glUniform1f(glGetUniformLocation(id, (uniformName+"base.diffuse_intensity").c_str()), diffuse_intensity);

    glUniform3f(glGetUniformLocation(id, (uniformName+"local_position").c_str()), local_pos.x,local_pos.y,local_pos.z);
    glUniform1f(glGetUniformLocation(id, (uniformName+"constant_attenuation").c_str()), constant_att);
    glUniform1f(glGetUniformLocation(id, (uniformName+"linear_attenuation").c_str()), linear_att);
    glUniform1f(glGetUniformLocation(id, (uniformName+"exponential_attenuation").c_str()), exponential_att);
}
void Shader::setSpotLight(int index, glm::vec3 color, float ambient_intensity, float diffuse_intensity, glm::vec3 local_pos, float constant_att, float linear_att, float exponential_att,  glm::vec3 local_direction, float cutoff) const
{
    std::string uniformName = "spot_lights[" + std::to_string(index) + "].";
    glUniform3f(glGetUniformLocation(id, (uniformName+"base.base.color").c_str()), color.r,color.g,color.b);
    glUniform1f(glGetUniformLocation(id, (uniformName+"base.base.ambient_intensity").c_str()), ambient_intensity);
    glUniform1f(glGetUniformLocation(id, (uniformName+"base.base.diffuse_intensity").c_str()), diffuse_intensity);

    glUniform3f(glGetUniformLocation(id, (uniformName+"direction").c_str()), local_direction.x,local_direction.y,local_direction.z);
    glUniform1f(glGetUniformLocation(id, (uniformName+"cutoff").c_str()), cosf(glm::radians(cutoff)));

    glUniform3f(glGetUniformLocation(id, (uniformName+"base.local_position").c_str()), local_pos.x,local_pos.y,local_pos.z);
    glUniform1f(glGetUniformLocation(id, (uniformName+"base.constant_attenuation").c_str()), constant_att);
    glUniform1f(glGetUniformLocation(id, (uniformName+"base.linear_attenuation").c_str()), linear_att);
    glUniform1f(glGetUniformLocation(id, (uniformName+"base.exponential_attenuation").c_str()), exponential_att);
}
void Shader::setMaterial(const std::string &name, Material& value) const
{
    const std::string& ambient = (name+ ".ambient_color");
    const std::string& diffuse = (name+ ".diffuse_color");
    const std::string& specular = (name+ ".specular_color");

    glUniform3f(glGetUniformLocation(id, ambient.c_str()), value.ambient_color.r,value.ambient_color.g,value.ambient_color.b);
    glUniform3f(glGetUniformLocation(id, diffuse.c_str()), value.diffuse_color.r,value.diffuse_color.g,value.diffuse_color.b);
    glUniform3f(glGetUniformLocation(id, specular.c_str()), value.specular_color.r,value.specular_color.g,value.specular_color.b);
}

