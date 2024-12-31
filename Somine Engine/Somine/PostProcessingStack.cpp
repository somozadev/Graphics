#include "PostProcessingStack.h"
#include <iostream>
#include <stdexcept>

PostProcessingStack::PostProcessingStack(GLuint w, GLuint h): m_width(w), m_height(h)
{
    initQuad();
    initFramebuffers();
}

PostProcessingStack::~PostProcessingStack()
{
    glDeleteFramebuffers(1, &m_fboA);
    glDeleteFramebuffers(1, &m_fboB);
    glDeleteTextures(1, &m_textureA);
    glDeleteTextures(1, &m_textureB);
}

void PostProcessingStack::initFramebuffers()
{
    glGenFramebuffers(1, &m_fboA);
    glGenFramebuffers(1, &m_fboB);
    glGenTextures(1, &m_textureA);
    glGenTextures(1, &m_textureB);
    glGenRenderbuffers(1, &m_renderbuffer);

    auto setupFramebuffer = [&](GLuint fbo, GLuint texture)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Framebuffer is not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    };

    setupFramebuffer(m_fboA, m_textureA);
    setupFramebuffer(m_fboB, m_textureB);
}

void PostProcessingStack::initQuad()
{
    float quadVertices[] = {
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };

    unsigned int quadIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glGenBuffers(1, &m_quadEBO);

    glBindVertexArray(m_quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}


void PostProcessingStack::addEffect(const Shader* shader, const std::string& name)
{
    if (!shader)
        throw std::runtime_error("Shader is null for effect: " + name);
    m_effects.emplace_back(*shader, name);
}


GLuint PostProcessingStack::apply(GLuint inputTexture) const
{
    GLuint readFBO = m_fboA;
    GLuint writeFBO = m_fboB;

    for (const auto& effect : m_effects)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, writeFBO);
        glClear(GL_COLOR_BUFFER_BIT);

        effect.shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTexture);
        effect.shader.setInt("screen_texture", 0);
        //inject the needed extra vars ?
        renderQuad();

        std::swap(readFBO, writeFBO);
        inputTexture = (readFBO == m_fboA) ? m_textureA : m_textureB;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return inputTexture;
}


void PostProcessingStack::resize(GLuint width, GLuint height)
{
    m_width = width;
    m_height = height;

    initFramebuffers();
}


void PostProcessingStack::renderQuad() const
{
    glBindVertexArray(m_quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
