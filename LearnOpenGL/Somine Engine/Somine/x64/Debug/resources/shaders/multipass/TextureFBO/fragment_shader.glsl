#version 330 core
out vec4 fragment_color;
in vec2 tex_coords;     

uniform sampler2D source_texture;


void main()
{
     fragment_color =texture(source_texture, tex_coords);
    
  // fragment_color = vec4(1.0, 0.0, 0.0, 1.0);

}