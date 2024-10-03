#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 uMVP;
out vec3 custom_colour;

void main()
{
    gl_Position = uMVP * vec4(vertex_position, 1.0);
    custom_colour = vertex_colour; 
}