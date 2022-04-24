//
//  resource_manager.cpp
//  BreakOutES
//
//  Created by chen caibin on 2021/10/28.
//

#include "resource_manager.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader &ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader &ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D &ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D &ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (Properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader &ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    Shader *shader = new Shader(vShaderFile,fShaderFile);
    return *shader;
}

Texture2D &ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    // Create Texture object
    Texture2D *texture = new Texture2D();
    if (alpha)
    {
        texture->Internal_Format = GL_RGBA;
        texture->Image_Format = GL_RGBA;
    }
    // Load image
    int width, height, nrComponents;
    stbi_convert_iphone_png_to_rgb(1);
    unsigned char* image = stbi_load(file, &width, &height, &nrComponents, 0);
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    else
        format = GL_RED;
    
    texture->Internal_Format = format;
    texture->Image_Format = format;
    
    // Now generate texture
    texture->Generate(width, height, image);
    // And finally free image data
    stbi_image_free(image);
    return *texture;
}
