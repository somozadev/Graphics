#version 330 core

uniform sampler2D scene_texture;   
uniform vec2 screen_resolution;   
uniform float vignette_radius;    // 0.4-0.8
uniform float vignette_softness;  // 0.1 - 0.3
uniform float vignette_intensity; // 0.5 - 1.0

in vec2 tex_coords;
out vec4 fragment_color;

void main() {
    // Step 1: Calcula las coordenadas relativas al centro de la pantalla
    vec2 screen_coords = tex_coords * screen_resolution;
    vec2 center = screen_resolution * 0.5;
    float distance = length((screen_coords - center) / screen_resolution);

    // Step 2: Calcula el factor de oscurecimiento (invertido para aplicar en los bordes)
    float vignette = smoothstep(vignette_radius, vignette_radius - vignette_softness, distance);

    // Invertir el valor de vignette para que los bordes sean oscuros
    vignette = 1.0 - vignette;

    // Step 3: Combina el efecto de vignette con la escena original
    vec3 scene_color = texture(scene_texture, tex_coords).rgb;
    fragment_color = vec4(scene_color * (1.0 - vignette * vignette_intensity), 1.0);
}