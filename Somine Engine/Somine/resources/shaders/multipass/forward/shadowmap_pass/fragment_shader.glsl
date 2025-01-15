#version 330 core   

out vec4 fragment_color;
in vec2 tex_coords;   


uniform sampler2D texture_shadowmap; 


void main()
{
    float depth = texture(texture_shadowmap, tex_coords).r;
    fragment_color = vec4(depth,depth,depth, 1.0);

}

