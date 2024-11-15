#version 330 core
const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;

out vec4 fragment_color;

in vec2 tex_coords;   


uniform sampler2D texture_shadowmap; 


void main()
{
    float depth = texture(texture_shadowmap, tex_coords).r;
    depth = 1.0 - (1.0 - depth) * 25.0; 
    fragment_color = vec4(depth, 1.0);

}

