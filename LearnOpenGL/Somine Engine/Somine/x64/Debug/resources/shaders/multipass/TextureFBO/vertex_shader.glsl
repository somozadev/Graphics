#version 330 core
layout (location = 0) in vec2 vertex_position;

out vec2 tex_coords;

void main()
{
    gl_Position =vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);
    tex_coords = (vertex_position + vec2(1,1)) * 0.5;    
}