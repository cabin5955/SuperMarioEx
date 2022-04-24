//
//  animation2d.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/3.
//

#ifndef animation2d_hpp
#define animation2d_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "texture.hpp"

class Animation2D{
private:
    std::string _basePath;
    unsigned _frameCount;
    float _timer;
    bool _loop;
public:
    Animation2D(std::string basePath, unsigned frameCount, bool loop);
    void Preload();
    unsigned SetTimer(float time);
    std::string GetCurrentFrame();
    bool Finished();
};


#endif /* animation2d_hpp */
