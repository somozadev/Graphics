#pragma once
#include <unordered_map>
#include <vector>
#include <glad/glad.h>

#include "Camera.h"
#include "Helper.h"
#include "Macros.h"
#include "OldMesh.h"
#include "PostProcessingStack.h"
#include "Shader.h"
#include "Window.h"
#include "assimpLoader/Model.h"
#include "Lights/BaseLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "primitives/CubePrimitive.h"
#include "primitives/PlanePrimitive.h"
#include "primitives/PyramidPrimitive.h"
#include "primitives/SpherePrimitive.h"
#include "primitives/Terrain.h"
#include "primitives/grid/Grid.h"

class Renderer
{
private:
    OldMesh m_grid;
    Camera m_camera;
    std::unordered_map<std::string, const Shader*> m_shaders;
    // PostProcessingStack m_postProcessingStack;
    std::vector<Model*> m_models;

    float m_current_frame{};
    float m_last_frame{};

    std::vector<GLfloat> m_bg_color;
    bool m_wireframe{false};

    bool m_dof{false}; //'with the bottom config you get a cool sharp close-up dof
    float m_dof_focus_distance{10.0f}; // 0.1
    float m_dof_focus_range{2.0f}; // 4.375
    float m_dof_blur_strength{1.0f}; // 10 

    
    bool m_pixelize{false};
    float pixel_size{10.0f};
    
    bool m_vignette{false};
    float m_vignette_radius{0.5f};    
    float m_vignette_softness{0.2f};  
    float m_vignette_intensity{0.8f};
    
    bool m_antialiasing{false};
    int m_antialiasing_debug_mode{0};
    float m_antialiasing_mul_reduce {8.0f};
    float m_antialiasing_min_reduce {128.0f};
    float m_antialiasing_luma_threshold {0.05f};
    int m_antialiasing_max_span {8};
    
    bool m_cell_shading{false};
    bool m_greyscale_shading{false};
    bool m_dnw_shading{false};
    int m_cell_shading_levels{2};
    bool m_see_grid{false};

    DirectionalLight* m_light = NEW(DirectionalLight);
    std::vector<PointLight*> m_point_lights; 
    std::vector<SpotLight*> m_spot_lights; 

    GLuint m_postprocessing_A_FBO{0};
    GLuint m_postprocessing_texture_A{0};
    GLuint m_postprocessing_B_FBO{0};
    GLuint m_postprocessing_texture_B{0};


    GLuint m_FBO{0};
    GLuint m_textureFBO{0}; //final rendered texture
        
    GLuint m_quadMeshVAO{0};
    GLuint m_RBO{0}; //rbo for depth and stencil 
    GLuint m_depthTexture{0};
    
    GLuint m_shadowmap_FBO {0};
    GLuint m_shadowmap_texture_FBO {0};

    int m_shadow_resolution{4};
    
    void drawGrid(glm::mat4 projection, glm::mat4 view);
    void setupMatrices(glm::mat4 projection, glm::mat4 view);
    void calcDeltaTime();

    void depthPass();
    void stencilPass();
    void lightPass();
    void colorPass();
    void shadowmapPass(); 
public:
    Window* m_window;
    float m_delta_time{};
    float m_current_ms{};
    Renderer(Window* window = nullptr);
    void init();
    void initModels();
    void initFramebuffer();
    void initShadowmap();
    void initShadersMap();
    void update();    
    void resizeFramebuffer(int width, int height);
    ~Renderer();
};
