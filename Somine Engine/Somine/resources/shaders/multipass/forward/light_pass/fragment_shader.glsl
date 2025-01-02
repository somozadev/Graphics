#version 330 core
const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;

out vec4 fragment_color;

in vec2 tex_coords;     
in vec3 vertex_normal; 
in vec3 position; 
in vec4 light_space_pos;


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
    vec3 local_position; 
    float constant_attenuation; 
    float linear_attenuation; 
    float exponential_attenuation; 
};
struct SpotLight
{
    PointLight base; 
    vec3 direction; 
    float cutoff; 
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
uniform int n_spot_lights;
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

uniform Material material;
uniform vec3 camera_local_position;

uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_normal1; 
uniform sampler2D texture_specular1; 
uniform sampler2D specular_exponent1; 

uniform sampler2D shadowmap; 

uniform bool use_cell_shading = false; 
uniform bool use_greyscale_shading = false; 
uniform bool use_dnw_shading = false;
uniform int cell_shading_levels = 2; 


float Quantize(float value, int levels)
{
    return floor(value * levels) / levels;
}
float CalcShadow(vec4 light_space_pos)
{
    vec3 proj_coords = light_space_pos.xyz / light_space_pos.w;
    proj_coords = proj_coords * 0.5 + 0.5;

    float bias = 0.005;
    float closestDepth = texture(shadowmap, proj_coords.xy).r; 
    float currentDepth = proj_coords.z- bias;
   
    float shadow = 0.0;
    float texelSize = 1.0 / 2048.0; 
    for (int x = -1; x <= 1; ++x) { //PCF (Percentage Closer Filtering)
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowmap, proj_coords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; 
        if(proj_coords.z > 1.0)
        shadow = 0.0;
    return shadow;
    
 //   vec3 proj_coords = light_space_pos.xyz / light_space_pos.w; 
 //   proj_coords = proj_coords * 0.5 + 0.5; 
//
 //   if (proj_coords.z > 1.0 || proj_coords.x < 0.0 || proj_coords.x > 1.0 || proj_coords.y < 0.0 || proj_coords.y > 1.0)
 //       return 0.0;
//
 //   float current_depth = proj_coords.z;
 //   float closest_depth = texture(shadowmap, proj_coords.xy).r;
 //   float bias = 0.005;
//
 //   return current_depth > closest_depth + bias ? 1.0 : 0.0;
}

vec4 CalcLightInternally(BaseLight base, vec3 direction, vec3 normal, float shadow_factor)
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
            return ((ambient_color + (1.0 - shadow_factor)) * diffuse_color);
    }
    else
    {
        vec4 specular_color = vec4(0, 0, 0, 0); 
        if(diffuse_factor > 0)
        {
            diffuse_color = vec4(base.color,1.0) * base.diffuse_intensity * vec4(material.diffuse_color,1.0) * diffuse_factor;
            vec3 pixel_to_camera = normalize(camera_local_position - position); 
            vec3 light_reflect = normalize(reflect(-direction, normal));
            float specular_factor = dot(pixel_to_camera, light_reflect); //
            //pow(max(dot(pixel_to_camera, light_reflect), 0.0), material.shininess) to add shininesss to materials
            if(specular_factor > 0)
            {
                float specular_exponent = texture(specular_exponent1, tex_coords).r * 255.0; 
                specular_factor = pow(specular_factor, specular_exponent); 
                specular_color = vec4(base.color, 1.0) * base.diffuse_intensity * vec4(material.specular_color, 1.0) * specular_factor; 
            } 
        //  vec3 emission = texture(material.emission_color, tex_coords).rgb;
        }     
        return((ambient_color + (1.0 - shadow_factor))  * (diffuse_color + specular_color));//+emission
    }     
}

vec4 CalcDirectionalLight(vec3 normal, float shadow_factor)
{
    return CalcLightInternally(directional_light.base, directional_light.direction, normal, shadow_factor);
}
vec4 CalcPointLight(PointLight point_light, vec3 normal)
{
    vec3 light_direction =  position - point_light.local_position;   
    float distance = length(light_direction);
    light_direction = normalize(light_direction); 
    
    vec4 color = CalcLightInternally(point_light.base, light_direction, normal, 0);
    float attenuation = point_light.constant_attenuation + point_light.linear_attenuation * distance + point_light.exponential_attenuation * distance * distance; 
    
    return color/attenuation; 
}
vec4 CalcSpotLight(SpotLight spot_light, vec3 normal)
{
    vec3 light_to_pixel = normalize(position - spot_light.base.local_position);   
    float spot_factor = dot(light_to_pixel, spot_light.direction);
    if(spot_factor > spot_light.cutoff)
    {
        vec4 color = CalcPointLight(spot_light.base,normal); 
        float spot_light_intensity = (1.0 - (1.0 - spot_factor) / (1.0 - spot_light.cutoff)); 
        return color * spot_light_intensity; 
    }
    else
        return vec4(0.0,0.0,0.0,0.0);
    
}
void main()
{

    vec3 norm_normal = normalize(vertex_normal);
    float shadow_factor = CalcShadow(light_space_pos);
    vec4 total_lightning = CalcDirectionalLight(norm_normal, shadow_factor); 
    for(int i=0; i < n_point_lights; i++)
    {
        total_lightning += CalcPointLight(point_lights[i], norm_normal);
    }
    for(int i=0; i < n_spot_lights; i++)
    {
        total_lightning += CalcSpotLight(spot_lights[i], norm_normal);
    }
    vec3 color;
    if(use_greyscale_shading)
    {
        color = total_lightning.rgb;
    }
    else if(use_dnw_shading)
    {
        vec3 clamped_lightning = step(0.5, total_lightning.rgb);
        color = clamped_lightning;
    }
    else
        color = texture(texture_diffuse1, tex_coords).rgb * total_lightning.rgb;
    fragment_color = vec4(color, 1.0);

}

