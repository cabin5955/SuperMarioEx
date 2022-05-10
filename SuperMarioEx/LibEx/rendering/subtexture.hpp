//
//  subtexture.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/7.
//

#ifndef subtexture_hpp
#define subtexture_hpp

#include "texture.hpp"
#include <glm/glm.hpp>

class SubTexture2D
{
public:
    SubTexture2D(const Texture2D& texture, const glm::vec2& min, const glm::vec2& max);
    const Texture2D GetTexture()  { return m_texture; }
    const glm::vec2* GetTexCoords()  { return m_TexCoords;}
    
    static SubTexture2D* CreateFromCoords(const Texture2D& texture,
                                         const glm::vec2& coords,
                                         const glm::vec2& cellSize,
                                         const glm::vec2& spriteSize = {1,1});
private:
    Texture2D m_texture;
    glm::vec2 m_TexCoords[4];
};


#endif /* subtexture_hpp */
