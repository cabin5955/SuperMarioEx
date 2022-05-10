//
//  sprites_animation.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/3.
//

#ifndef sprites_animation_hpp
#define sprites_animation_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "texture.hpp"

class SpritesAnimation{
private:
    std::string _basePath;
    unsigned _frameCount;
    float _timer;
    bool _loop;
public:
    SpritesAnimation(std::string basePath, unsigned frameCount, bool loop);
    void Preload();
    unsigned SetTimer(float time);
    std::string GetCurrentFrame();
    bool Finished();
};


#endif /* sprites_animation_hpp */
