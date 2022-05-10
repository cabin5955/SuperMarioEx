//
//  spritesheets_animation.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/9.
//

#include "spritesheets_animation.hpp"
#include "utils.h"

SpriteSheetsAnimation::SpriteSheetsAnimation(const Texture2D& texture, bool loop)
:m_texture(texture){
    _frameCount = 0;
    _timer = 0;
    _loop = loop;
}

void SpriteSheetsAnimation::addFrame(const glm::vec2& coords,
                                     const glm::vec2& cellSize,
                                     const glm::vec2& spriteSize)
{
    m_frames.push_back(SubTexture2D::CreateFromCoords(m_texture, coords, cellSize, spriteSize));
    _frameCount++;
}

unsigned SpriteSheetsAnimation::SetTimer(float time){
    if(!_loop && time > _frameCount-1){
        _timer = _frameCount-1;
    }else{
        _timer = fModulus(time,_frameCount);
    }
    return _timer;
}

const SubTexture2D& SpriteSheetsAnimation::GetCurrentFrame(){
    int frame = (int)_timer;
    return *m_frames[frame];
}

bool SpriteSheetsAnimation::Finished(){
    if(_loop) return false;
    return ((unsigned)_timer == _frameCount-1);
}
