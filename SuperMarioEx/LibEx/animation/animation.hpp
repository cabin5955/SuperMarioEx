//
//  animation.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/2/19.
//

#ifndef animation_hpp
#define animation_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "model.hpp"

class Animation{
    private:
        std::string _basePath;
        unsigned _frameCount;
        float _timer;
        bool _loop;
        Model model;
    public:
        Animation(std::string basePath, unsigned frameCount, bool loop);
        void Preload();
        unsigned SetTimer(float time);
        std::string GetCurrentFrame();
        bool Finished();
};

#endif /* animation_hpp */
