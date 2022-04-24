//
//  animation.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/2/19.
//

#include "animation.hpp"
#include <sstream>
#include "utils.h"
#include "model_cache.hpp"

Animation::Animation(std::string basePath, unsigned frameCount, bool loop){
    _basePath = basePath;
    _frameCount = frameCount;
    _timer = 0;
    _loop = loop;
}

void Animation::Preload(){
    for(unsigned i=0; i<_frameCount; i++){
        std::ostringstream stream;
        stream << _basePath << "_" << i << ".obj";
        ModelCache::CacheModel(stream.str());
    }
}

unsigned Animation::SetTimer(float time){
    if(!_loop && time > _frameCount-1){
        _timer = _frameCount-1;
    }else{
        _timer = fModulus(time,_frameCount);
    }
    return _timer;
}

std::string Animation::GetCurrentFrame(){
    int frame = (int)_timer;
    std::ostringstream stream;
    stream << _basePath << "_" << frame << ".obj";
    return stream.str();
}

bool Animation::Finished(){
    if(_loop) return false;
    return ((unsigned)_timer == _frameCount-1);
}
