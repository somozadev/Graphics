#version 330 core
const int MAX_POINT_LIGHTS = 10;

out vec4 fragment_color;

in vec2 tex_coords;     
in vec3 vertex_normal; 
in vec3 position; 


struct BaseLight
{
    vec3 color; 
    float ambient_intensity; 
    float diffuse_intensity; 
};
struct DirectionalLight
{
    BaseLight base;
    vec3 direction; 
};
struct PointLight
{
    BaseLight base; 
    vec3 local_poition; 
    float constant_attenuation; 
    float linear_attenuation; 
    float exponential_attenuation; 
};

struct Material
{ 
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

uniform DirectionalLight directional_light;
uniform int n_point_lights;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

uniform Material material;
uniform vec3 camera_local_position;

uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_normal1; 
uniform sampler2D texture_specular1; 
uniform sampler2D specular_exponent1; 

uniform bool use_cell_shading = false; 
uniform int cell_shading_levels = 2; 


float Quantize(float value, int levels)
{
    return floor(value * levels) / levels;
}

vec4 CalcLightInternally(BaseLight base, vec3 direction, vec3 normal)
{
    vec4 ambient_color = vec4(base.color, 1.0) * base.ambient_intensity * vec4(material.ambient_color, 1.0) ;
    float diffuse_factor = dot(normal, -direction); 
    vec4 diffuse_color = vec4 (0, 0, 0, 0); 
    
    if (use_cell_shading)
    {
        diffuse_factor = Quantize(max(diffuse_factor, 0.0), cell_shading_levels);
            if (diffuse_factor > 0)
            {
                diffuse_color = vec4(base.color, 1.0) * base.diffuse_intensity * vec4(material.diffuse_color, 1.0) * diffuse_factor;
            }
            return (ambient_color + diffuse_color); 
    }
    else
    {
        vec4 specular_color = vec4(0, 0, 0, 0); 
        if(diffuse_factor > 0)
        {
            diffuse_color = vec4(base.color,1.0) * base.diffuse_intensity * vec4(material.diffuse_color,1.0) * diffuse_factor;
            vec3 pixel_to_camera = normalize(camera_local_position - position); 
            vec3 light_reflect = normalize(reflect(direction, normal));
            float specular_factor = dot(pixel_to_camera, light_reflect); 
            if(specular_factor > 0)
            {
                float specular_exponent = texture(specular_exponent1, tex_coords).r * 255.0; 
                specular_factor = pow(specular_factor, specular_exponent); 
                specular_color = vec4(base.color, 1.0) * base.diffuse_intensity * vec4(material.specular_color, 1.0) * specular_factor; 
            } 
        
        }     
        return(ambient_color + diffuse_color + specular_color); 
    }     
}

vec4 CalcDirectionalLight(vec3 normal)
{
    return CalcLightInternally(directional_light.base, directional_light.direction, normal);
}
vec4 CalcPointLight(int i, vec3 normal)
{
    vec3 light_direction = point_lights[i].local_poition - position;   
    float distance = length(light_direction);
    light_direction = normalize(light_direction); 
    
    vec4 color = CalcLightInternally(point_lights[i].base, light_direction, normal);
    float attenuation = point_lights[i].constant_attenuation + point_lights[i].linear_attenuation * distance + point_lights[i].exponential_attenuation * distance * distance; 
    
    return color/attenuation; 
}
void main()
{

    vec3 norm_normal = normalize(vertex_normal);
    vec4 total_lightning = CalcDirectionalLight(norm_normal); 
    for(int i=0; i < n_point_lights; i++)
    {
        total_lightning += CalcPointLight(i, norm_normal);
    }
    vec3 color = texture(texture_diffuse1, tex_coords).rgb * total_lightning.rgb;
    fragment_color = vec4(color, 1.0);

}

