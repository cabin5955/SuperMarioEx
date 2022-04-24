//
//  model_cache.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/9.
//

#ifndef model_cache_hpp
#define model_cache_hpp

#include <string>
#include <list>
#include <map>
#include "model.hpp"

class ModelCache{
private:
    static std::map<std::string, Model*> modelsCache;
    
public:
    static void CacheModel(const std::string modelPath);
    static Model* LoadModel(const std::string modelPath);
};

#endif /* model_cache_hpp */
