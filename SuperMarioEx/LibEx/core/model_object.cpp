//
//  model_object.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/10.
//

#include "model_object.hpp"
#include "color.h"
#include "model_cache.hpp"

ModelObject::ModelObject(){
    color = {255,255,255,255};
    Position = glm::vec3(0.0f);
    Size = {1,1,1};
    RotationAxis = {0,1,0};
    RotationAngle = 0;
}

ModelObject::ModelObject(const std::string modelPath, Color c) : GameObject(){
    model = ModelCache::LoadModel(modelPath);
    color = c;
    Position = glm::vec3(0.0f);
    Size = {1,1,1};
    RotationAxis = {0,1,0};
    RotationAngle = 0;
}

void ModelObject::SetColor(Color c)
{
    color = c;
}
