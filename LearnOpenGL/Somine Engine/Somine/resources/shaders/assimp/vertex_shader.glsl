#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normals;
layout (location = 2) in vec2 vertex_tex_coord;

out vec2 tex_coords;
out vec3 vertex_normal;
out vec3 position; 

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    tex_coords = vertex_tex_coord;
    vertex_normal = vertex_normals; 
    position = vertex_position; 
}