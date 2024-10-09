#version 330 core
out vec4 fragment_color;

in vec2 tex_coords;     

uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_normal1; 
uniform sampler2D texture_specular1; 



void main()
{
    
    vec3 normal_map = texture(texture_normal1, tex_coords).rgb;
     normal_map = normalize(normal_map * 2.0 - 1.0); 
   
    vec3 diffuse_texture = texture(texture_diffuse1, tex_coords).rgb;
      float specular_texture = texture(texture_specular1, tex_coords).r;
      
    fragment_color = vec4(texture(texture_diffuse1, tex_coords)); 

}