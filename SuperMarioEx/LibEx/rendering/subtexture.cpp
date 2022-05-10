//
//  subtexture.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/7.
//

#include "subtexture.hpp"

SubTexture2D::SubTexture2D(const Texture2D& texture, const glm::vec2& min, const glm::vec2& max)
:m_texture(texture)
{
    m_TexCoords[0] = {min.x, min.y};
    m_TexCoords[1] = {max.x, min.y};
    m_TexCoords[2] = {max.x, max.y};
    m_TexCoords[3] = {min.x, max.y};
}

SubTexture2D* SubTexture2D::CreateFromCoords(const Texture2D& texture,
                                     const glm::vec2& coords,
                                     const glm::vec2& cellSize,
                                     const glm::vec2& spriteSize)
{
    glm::vec2 min = { (coords.x * cellSize.x ) / texture.Width, (coords.y * cellSize.y ) / texture.Height};
    glm::vec2 max = { (( coords.x + spriteSize.x ) * cellSize.x ) / texture.Width,
                      (( coords.y + spriteSize.y ) * cellSize.y ) / texture.Height
                    };
    
    return new SubTexture2D(texture, min, max);
}
