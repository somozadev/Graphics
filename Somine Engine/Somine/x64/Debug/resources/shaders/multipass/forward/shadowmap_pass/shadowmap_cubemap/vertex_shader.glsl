#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normals;
layout (location = 2) in vec2 vertex_tex_coord;

out vec3 fragment_position;

uniform mat4 shadow_matrices[6];

void main()
{
    fragment_position = vertex_position;
    gl_Position = shadow_matrices[gl_VertexID] * vec4(vertex_position, 1.0);
}