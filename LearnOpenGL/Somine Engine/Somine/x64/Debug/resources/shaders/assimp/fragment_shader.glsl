#version 330 core
out vec4 fragment_color;

in vec2 tex_coords;     
//in vec3 fragment_position;     
//in vec3 normal; 

uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_normal1; 
uniform sampler2D texture_specular1; 



void main()
{
   // vec3 light_color = vec3(1.0, 1.0, 1.0); 
   // vec3 objects_color = vec3(1.0, 1.0, 1.0); 
   // float ambient_strength = 0.1;
   // vec3 ambient = ambient_strength * light_color;

  //  vec3 light_pos_hardcoded = vec3(0.0,0.0,20.0);  
   // vec3 norm = normalize(mormal);
   // vec3 light_direction = normalize(light_pos_hardcoded - fragment_position); 
  //  float diff = max(dot(norm, light_direction), 0.0); 
  //  vec3 diffuse = diff * light_color; 
  //  vec3 result = (ambient + diffuse) * object_color;
    
    
     //vec3 normal_map = texture(texture_normal1, tex_coords).rgb;
     //normal_map = normalize(normal_map * 2.0 - 1.0); 
    //float specular_texture = texture(texture_specular1, tex_coords).r;
    
    vec3 diffuse_texture = texture(texture_diffuse1, tex_coords).rgb;
      
    if (diffuse_texture == vec3(0.0, 0.0, 0.0)) 
    {
        fragment_color = vec4(1.0,1.0,1.0, 1.0); 
    } 
    else 
    {
        fragment_color = vec4(texture(texture_diffuse1, tex_coords)); 
    }

}