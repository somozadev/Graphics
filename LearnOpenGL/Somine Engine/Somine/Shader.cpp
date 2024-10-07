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


Shader::Shader()
{
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