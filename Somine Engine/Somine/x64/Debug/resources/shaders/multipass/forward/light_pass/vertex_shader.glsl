#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normals;
layout (location = 2) in vec2 vertex_tex_coord;

out vec2 tex_coords;
out vec3 vertex_normal;
out vec3 position; 
out vec4 light_space_pos; 

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 light_view_projection; 

void main()
{
    
    position =vec3(model * vec4(vertex_position, 1.0)); //model * vec4(vertex_position, 1.0);
    vertex_normal = transpose(inverse(mat3(model))) * vertex_normals;// vertex_normals; 
    tex_coords = vertex_tex_coord;
    light_space_pos = light_view_projection * vec4(position, 1.0);//* world_position* vec4(vertex_position, 1.0); 
        
    gl_Position = projection * view * vec4(position, 1.0);
    
    
}