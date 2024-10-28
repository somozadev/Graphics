#version 330 core
out vec4 fragment_color;

in vec2 tex_coords;     
in vec3 normal; 

uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_normal1; 
uniform sampler2D texture_specular1; 

/*struct BaseLight
{
    vec3 color; 
    float ambient_intensity; 
};*/
struct DirectionalLight
{
    vec3 color; 
    float ambient_intensity; 
    float diffuse_intensity; 
    vec3 direction; 
};
struct Material
{ 
    vec3 ambient_color;
    vec3 diffuse_color;
};

uniform DirectionalLight directional_light;
uniform Material material;


void main()
{
    DirectionalLight directional_lightt;
    directional_lightt.color = vec3(1.0, 1.0, 1.0); // Luz blanca
    directional_lightt.ambient_intensity = 0.2; // Luz ambiental
    directional_lightt.diffuse_intensity = 1.0; // Luz difusa
    directional_lightt.direction = vec3(0.0, -1.0, -1.0); 
    Material materiall;
    materiall.ambient_color = vec3(1.0, 1.0, 1.0); // Blanco
    materiall.diffuse_color = vec3(1.0, 1.0, 1.0); 
    vec4 ambient_color = vec4(directional_light.color, 1.0) 
                         * directional_light.ambient_intensity 
                         * vec4(materiall.ambient_color, 1.0);
                             
                             

   float diffuse_factor = max(dot(normalize(normal), -normalize(directional_light.direction)), 0.0); 
   vec4 diffuse_color = vec4 (0, 0, 0, 0); 
    
if(diffuse_factor > 0)
    {
        diffuse_color = vec4(directional_light.color, 1.0) 
                        * directional_light.diffuse_intensity 
                        * vec4(materiall.diffuse_color, 1.0) 
                        * diffuse_factor;
    }
    
    vec3 color = texture(texture_diffuse1, tex_coords).rgb * (ambient_color.rgb + diffuse_color.rgb);
    fragment_color = vec4(color, 1.0);
    //fragment_color = vec4((texture(texture_diffuse1,  tex_coords).rgb * light_color * ambient_strength), 1.0); 
    //fragment_color = vec4(texture(texture_diffuse1, tex_coords).rgb, 1.0); test if textures are loaded correctly - works 
}























































/*

   vec3 light_color = vec3(1.0, 1.0, 1.0); 
   vec3 object_color = vec3(1.0,1.0,1.0); 
   float ambient_strength = 0.24;
   vec3 ambient = ambient_strength * light_color;
    vec3 diffuse_texture = texture(texture_diffuse1, tex_coords).rgb;

    fragment_color = vec4(diffuse_texture * ambient, 1.0); 

void debugLight()
{
vec3 light_direction = normalize(light_pos - fragment_position);
    fragment_color = vec4(light_direction, 1.0); 
}
    
    
    void defaultMain()
    {
      vec3 light_color = vec3(1.0, 1.0, 1.0); 
   vec3 object_color = vec3(1.0,1.0,1.0); 
   float ambient_strength = 0.24;
   vec3 ambient = ambient_strength * light_color;

   vec3 norm = normalize(normal);
   vec3 light_direction = normalize(light_pos - fragment_position); 
  
  float diff = max(dot(norm, light_direction), 0.0); 
  vec3 diffuse = diff * light_color; 
  vec3 result = (ambient + diffuse) * object_color;
    
    
     //vec3 normal_map = texture(texture_normal1, tex_coords).rgb;
     //normal_map = normalize(normal_map * 2.0 - 1.0); 
    //float specular_texture = texture(texture_specular1, tex_coords).r;
    
    vec3 diffuse_texture = texture(texture_diffuse1, tex_coords).rgb;
      
    // if (diffuse_texture == vec3(0.0, 0.0, 0.0)) 
    // {
   //      fragment_color = vec4(1.0,1.0,1.0, 1.0); 
    // } 
   //   else 
  //    {
        vec3 test = result * diffuse_texture;
        fragment_color = vec4(test, 1.0); 
    //  }
    }
    
    
    
void main()
{    
    
 //debugLight();
defaultMain();
}*/