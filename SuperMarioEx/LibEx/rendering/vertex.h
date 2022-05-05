//
//  Vertex.h
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/26.
//

#ifndef vertex_h
#define vertex_h

#include <glm/glm.hpp>

typedef struct
{
    glm::vec3   position;
    glm::vec4   color;
    glm::vec2   texCoords;
    glm::int32  texIndex;
} T_Vertex;

#endif /* vertex_h */
