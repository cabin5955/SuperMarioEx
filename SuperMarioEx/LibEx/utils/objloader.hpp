//
//  objloader.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/7.
//

#ifndef objloader_hpp
#define objloader_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace std;

bool loadOBJ(
    const char * path,
    std::vector<glm::vec3> & out_vertices,
    std::vector<glm::vec2> & out_uvs,
    std::vector<glm::vec3> & out_normals
);

#endif /* objloader_hpp */
