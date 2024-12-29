
#version 330 core

uniform sampler2D screen_texture;
uniform vec2 inverse_screen_size;
uniform int debug_mode; // 0: normal, 1: luminance, 2: edge detection, 3: search directions
uniform float FXAA_MUL_REDUCE;
uniform float FXAA_LUMA_THRESHOLD;
uniform float FXAA_MIN_REDUCE;
uniform float FXAA_MAX_SPAN;

in vec2 tex_coords;
out vec4 fragment_color;

void main() {
  
    vec2 offset = inverse_screen_size.xy;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaTL = dot(luma, texture2D(screen_texture, tex_coords.xy + (vec2(-1.0, -1.0) * offset)).xyz);
    float lumaTR = dot(luma, texture2D(screen_texture, tex_coords.xy + (vec2(1.0, -1.0) * offset)).xyz);
    float lumaBL = dot(luma, texture2D(screen_texture, tex_coords.xy + (vec2(-1.0, 1.0) * offset)).xyz);
    float lumaBR = dot(luma, texture2D(screen_texture, tex_coords.xy + (vec2(1.0, 1.0) * offset)).xyz);
    float lumaM = dot(luma, texture2D(screen_texture, tex_coords.xy).xyz);
    
    
    vec2 dir; 
    dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
    dir. y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));
    
    float dirReduce= max((lumaTL + lumaTR + lumaBL + lumaBR) * (FXAA_MUL_REDUCE * 0.25), FXAA_MIN_REDUCE); 
    float inverseDirectionAdjustment = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    
    dir = min(vec2(FXAA_MAX_SPAN,FXAA_MAX_SPAN),   max(vec2(-FXAA_MAX_SPAN,-FXAA_MAX_SPAN), dir * inverseDirectionAdjustment)) * offset;
    vec3 result1 = (1.0/2.0) * (texture2D(screen_texture, tex_coords.xy + (dir * vec2(1.0/3.0 - 0.5))).xyz + texture2D(screen_texture, tex_coords.xy + (dir * vec2(2.0/3.0 - 0.5))).xyz  );
    vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (texture2D(screen_texture, tex_coords.xy + (dir * vec2(0.0/3.0 - 0.5))).xyz + texture2D(screen_texture, tex_coords.xy + (dir * vec2(3.0/3.0 - 0.5))).xyz  );
    
    float lumaMin = min(lumaM, min(min(lumaTL,lumaTR), min(lumaBL, lumaBR)));
    float lumaMax = max(lumaM, max(max(lumaTL,lumaTR), max(lumaBL, lumaBR)));
    
    float lumaResult2 = dot(luma, result2);
     if (debug_mode == 1) {// Luminance debug mode
        
        float lumaDebug = dot(luma,texture2D(screen_texture, tex_coords.xy).xyz);
        fragment_color = vec4(vec3(lumaDebug), 1.0);
        return;
    } 
    else if(debug_mode == 2)
    {
        
    vec3 sobelHorizontal = textureOffset(screen_texture, tex_coords, ivec2(-1, 0)).rgb * -1.0 +
                           textureOffset(screen_texture, tex_coords, ivec2(1, 0)).rgb;

    vec3 sobelVertical = textureOffset(screen_texture, tex_coords, ivec2(0, -1)).rgb * -1.0 +
                         textureOffset(screen_texture, tex_coords, ivec2(0, 1)).rgb;

    // Calcula la magnitud del gradiente (fuerza del borde)
    float edgeStrength = length(sobelHorizontal + sobelVertical);
     if (edgeStrength > FXAA_LUMA_THRESHOLD) {
        fragment_color = vec4(1.0, 0.0, 0.0, 1.0);  // Rojo para bordes fuertes
    } else {
        fragment_color = vec4(0.0, 0.0, 1.0, 1.0);  // Azul para áreas sin bordes
        }  
        return;
    }
     else if (debug_mode == 3) {
        // Search direction debug mode
        vec3 dirColor = normalize(vec3(dir, 0.0));
        fragment_color = vec4(0.5 * dirColor + 0.5, 1.0); 
        return;
    }

    if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
        fragment_color = vec4(result1, 1.0);
    else 
        fragment_color = vec4(result2, 1.0);

    return;
    
    
    
}
