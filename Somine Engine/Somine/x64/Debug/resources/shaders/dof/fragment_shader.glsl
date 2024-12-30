
#version 330 core

uniform sampler2D color_texture;
uniform sampler2D depth_texture;
uniform vec2 inverse_screen_size;

//uniform int debug_mode; // 0: normal, 1: luminance, 2: edge detection, 3: search directions

uniform float DOF_FOCUS_DISTANCE;
uniform float DOF_FOCUS_RANGE;
uniform float DOF_BLUR_STRENGTH;

in vec2 tex_coords;
out vec4 fragment_color;

void main() {
  
    float depth = texture(depth_texture, tex_coords).r; 
    float pixel_distance = depth; //depth is already normalized
    
    //calc CoC
    float blur_amount = abs(pixel_distance - DOF_FOCUS_DISTANCE) / DOF_FOCUS_RANGE; 
    blur_amount = clamp(blur_amount, 0.0,1.0); 
    
    //stronger unfocus effect based on DOF_BLUR_STRENGTH VALUE
    float blur_radius = blur_amount * DOF_BLUR_STRENGTH; 
    
    vec3 color = vec3(0.0); 
    float total_weight = 0.0; 
    
    //perform aproximate unfocus based on multiple pixel samples
    for(int i=-4; i<= 4; i++)
    {
            for(int j=-4; j<= 4; j++)
            {
                vec2 offset = vec2(i,j) * inverse_screen_size * blur_radius;
                float weight = exp(-(i*i+j*j) / (2.0 * blur_radius * blur_radius ));
                color += texture(color_texture, tex_coords + offset).rgb * weight; 
                total_weight += weight;
            }
    }
    
    //normalize
    color /= total_weight;
    fragment_color = vec4(color, 1.0);
}
