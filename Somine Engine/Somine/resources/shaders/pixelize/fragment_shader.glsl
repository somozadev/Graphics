#version 330 core

in vec2 tex_coords;
out vec4 fragment_color;

uniform sampler2D color_texture;
uniform vec2 screen_resolution; 
uniform float pixel_size; 
void main()
{
    vec2 absolute_coords = tex_coords * screen_resolution;
    vec2 block_coords = floor(absolute_coords / pixel_size) * pixel_size;
    vec2 normalized_coords = block_coords / screen_resolution;
    vec4 color = texture(color_texture, normalized_coords);
    fragment_color = color;
}