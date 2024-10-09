#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normals;
layout (location = 2) in vec2 vertex_tex_coord;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in vec3 vertex_bit_tangent;
layout (location = 5) in vec3 vertex_bone_ids;
layout (location = 6) in vec3 vertex_weights;

out vec3 fragment_pos;   
out vec3 Normal;
out vec2 TexCoords; 
out mat3 TBN;

uniform mat4 model;                      
uniform mat4 view;                        
uniform mat4 projection; 

void main()
{
    fragment_pos = vec3(model * vec4(vertex_position, 1.0)); 
    Normal = mat3(transpose(inverse(model))) * vertex_normals; 
    TBN = transpose(mat3(model)) * mat3(vertex_tangent, vertex_bit_tangent, Normal);
    TexCoords = vertex_tex_coord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}