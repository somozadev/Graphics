#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include "Shader.h"

struct PostProcessingEffect
{
    Shader shader;
    std::string name;

    PostProcessingEffect(const Shader& shaderObj,
                         const std::string& effectName): name(effectName), shader(shaderObj)
    {
    }
};

class PostProcessingStack
{
private:
    GLuint m_fboA{}, m_fboB{};
    GLuint m_textureA{}, m_textureB{};
    GLuint m_width, m_height;

    GLuint m_renderbuffer{};
    GLuint m_quadVAO{}, m_quadVBO{}, m_quadEBO{};

    std::vector<PostProcessingEffect> m_effects;

    void initQuad();
    void initFramebuffers();

public:
    PostProcessingStack(GLuint w, GLuint h);
    ~PostProcessingStack();
    void addEffect(const Shader* shader, const std::string& name);
    GLuint apply(GLuint inputTexture) const;
    void resize(GLuint w, GLuint h);
    void renderQuad() const;
};


/*
 *  Not fully implemented due lack of time
 *  ideally, this will manage N amount of postprocessing effects
 *
 *  will continue with it after submitting 
 * 
 */