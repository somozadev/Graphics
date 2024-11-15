#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>


TextureManager::TextureManager()
{
}

unsigned int TextureManager::loadTexture(const char* path, GLenum format)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum imageFormat;
    
        if (nrChannels == 1)
        {
            internalFormat = GL_RED;
            imageFormat = GL_RED;
        }
        else if (nrChannels == 3)
        {
            internalFormat = GL_RGB;
            imageFormat = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            internalFormat = GL_RGBA;
            imageFormat = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << "\n";
    }
    stbi_image_free(data);

    return textureID;
}


TextureManager::~TextureManager()
{
}
