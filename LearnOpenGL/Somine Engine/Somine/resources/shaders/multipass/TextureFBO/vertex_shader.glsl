#version 330 core
layout (location = 0) in vec2 vertex_position;

out vec2 tex_coords;

void main()
{
    tex_coords = vertex_position * 0.5 + 0.5; 
    gl_Position =vec4(vertex_position, 0.0, 1.0);
}