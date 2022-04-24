//
//  model.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/8.
//

#ifndef model_hpp
#define model_hpp

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class  Model {
public:
    GLuint meshVAO;
    GLuint verticsSize;
    
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint uvbuffer;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    void PreLoad(const char *fileName);
};

#endif /* model_hpp */
