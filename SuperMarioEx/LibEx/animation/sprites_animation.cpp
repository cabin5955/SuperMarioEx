//
//  sprites_animation.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/3.
//

#include "sprites_animation.hpp"
#include <sstream>
#include "utils.h"
#include "resource_manager.hpp"
#include "global.hpp"

SpritesAnimation::SpritesAnimation(std::string basePath, unsigned frameCount, bool loop){
    _basePath = basePath;
    _frameCount = frameCount;
    _timer = 0;
    _loop = loop;
}

void SpritesAnimation::Preload(){
    char fullpath[1024] = {0};
    for(unsigned i=0; i<_frameCount; i++){
        std::ostringstream stream;
        int k = i+1;
        stream << _basePath << "-" << k << ".png";
        Global::ResFullPath(fullpath, stream.str().c_str());
        ResourceManager::LoadTexture(fullpath, GL_TRUE, stream.str());
        //printf("%s id %d\n",stream.str().c_str(),tex.ID);
    }
}

unsigned SpritesAnimation::SetTimer(float time){
    if(!_loop && time > _frameCount-1){
        _timer = _frameCount-1;
    }else{
        _timer = fModulus(time,_frameCount);
    }
    return _timer;
}

std::string SpritesAnimation::GetCurrentFrame(){
    int frame = (int)_timer;
    std::ostringstream stream;
    int k = frame+1;
    stream << _basePath << "-" << k << ".png";
    return stream.str();
}

bool SpritesAnimation::Finished(){
    if(_loop) return false;
    return ((unsigned)_timer == _frameCount-1);
}
