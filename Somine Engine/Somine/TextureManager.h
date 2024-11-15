#pragma once
#include <glad/glad.h>

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();
    unsigned static int loadTexture(const char* path, GLenum format);
    
};
