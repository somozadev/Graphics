#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normals;
layout (location = 2) in vec2 vertex_tex_coord;

out vec2 tex_coords;
out vec3 normal;
out vec3 fragment_position;
out vec3 light_pos; 

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 light_position;

void main()
{
    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
    tex_coords = vertex_tex_coord;
    normal = vertex_normals; 
    light_pos = light_position; 
    fragment_position = vec3(model*vec4(vertex_position, 1.0));
}