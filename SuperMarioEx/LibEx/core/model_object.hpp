//
//  model_object.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/10.
//

#ifndef model_object_hpp
#define model_object_hpp

#include "game_object.hpp"
#include "model.hpp"
#include "color.h"

class ModelObject:public GameObject {
  
public:
    Model *model;
    ModelObject();
    ModelObject(const std::string modelPath, Color c);
    void SetColor(Color c);
};


#endif /* model_object_hpp */
