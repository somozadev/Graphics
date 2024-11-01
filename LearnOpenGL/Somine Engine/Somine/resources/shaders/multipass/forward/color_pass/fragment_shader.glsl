#version 330 core
const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;

out vec4 fragment_color;

in vec2 tex_coords;     
in vec3 position; 


uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_normal1; 
uniform sampler2D texture_specular1; 
uniform sampler2D specular_exponent1; 


void main()
{
    vec3 color = texture(texture_diffuse1, tex_coords).rgb ;
    fragment_color = vec4(color, 1.0);

}

