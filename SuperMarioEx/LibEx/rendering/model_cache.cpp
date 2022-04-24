//
//  model_cache.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/9.
//

#include "model_cache.hpp"
#include "objloader.hpp"
#include "global.hpp"

std::map<std::string, Model*> ModelCache::modelsCache;

void ModelCache::CacheModel(const std::string modelPath){
    Model *model = new Model();
    char fullpath[1024] = {0};
    Global::ResFullPath(fullpath, modelPath.c_str());
    bool res = loadOBJ(fullpath, model->vertices, model->uvs, model->normals);
    model->verticsSize = (int)model->vertices.size();
    
    if(res)
    {
        glGenVertexArrays(1, &model->meshVAO);
        glBindVertexArray(model->meshVAO);
        
        glGenBuffers(1, &model->vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, model->vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec3), &model->vertices[0], GL_STATIC_DRAW);
        
        glGenBuffers(1, &model->normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, model->normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, model->normals.size() * sizeof(glm::vec3), &model->normals[0], GL_STATIC_DRAW);

        glGenBuffers(1, &model->uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, model->uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, model->uvs.size() * sizeof(glm::vec2), &model->uvs[0], GL_STATIC_DRAW);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        modelsCache[modelPath] = model;
    }
}

Model* ModelCache::LoadModel(const std::string modelPath){
    auto modelIt = modelsCache.find(modelPath);
    if(modelIt == modelsCache.end()){
        CacheModel(modelPath);
    }
    return modelsCache[modelPath];
}
