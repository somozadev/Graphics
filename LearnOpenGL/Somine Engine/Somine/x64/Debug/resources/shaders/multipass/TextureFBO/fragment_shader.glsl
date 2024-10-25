#version 330 core
out vec4 fragment_color;
in vec2 tex_coords;     

uniform sampler2D source_texture;


void main()
{
    vec3 my_texture =  texture(source_texture,tex_coords).rgb;
    fragment_color = vec4(my_texture, 1.0);

}