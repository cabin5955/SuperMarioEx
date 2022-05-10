//
//  spritesheets_animation.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/9.
//

#ifndef spritesheets_animation_hpp
#define spritesheets_animation_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "texture.hpp"
#include "subtexture.hpp"

class SpriteSheetsAnimation{
private:
    const Texture2D m_texture;
    std::vector<SubTexture2D*> m_frames;
    unsigned _frameCount;
    float _timer;
    bool _loop;
public:
    SpriteSheetsAnimation(const Texture2D& texture,bool loop);
    void addFrame(const glm::vec2& coords,
                  const glm::vec2& cellSize,
                  const glm::vec2& spriteSize = {1,1});
    unsigned SetTimer(float time);
    const SubTexture2D& GetCurrentFrame();
    bool Finished();
};


#endif /* spritesheets_animation_hpp */
