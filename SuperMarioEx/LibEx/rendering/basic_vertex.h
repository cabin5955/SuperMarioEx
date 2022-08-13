//
//  basic_vertex.h
//  SuperMarioEx
//
//  Created by chen caibin on 2022/7/9.
//

#ifndef basic_vertex_h
#define basic_vertex_h

#include <glm/glm.hpp>

typedef struct BasicVertex {
    glm::vec3   pos;
    glm::vec2   texCoord;
    glm::vec3   normal;
    
//    BasicVertex()
//    {
//        BasicVertex({0,0,0});
//    }
//
//    BasicVertex(glm::vec3 pos)
//    {
//        BasicVertex(pos, {0,0});
//    }
//
//    BasicVertex(glm::vec3 pos, glm::vec2 texCoord)
//    {
//        BasicVertex(pos, texCoord, {0,0,0});
//    }
//
//    BasicVertex(glm::vec3 pos, glm::vec2 texCoord, glm::vec3 normal)
//    {
//        this->pos = pos;
//        this->texCoord = texCoord;
//        this->normal = normal;
//    }
}BasicVertex;
#endif /* basic_vertex_h */
