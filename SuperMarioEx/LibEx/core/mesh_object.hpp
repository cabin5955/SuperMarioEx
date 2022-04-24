//
//  mesh_object.hpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/10/8.
//

#ifndef mesh_object_hpp
#define mesh_object_hpp

#include "glad.h"
#include <glm/glm.hpp>

#include "texture.hpp"
#include "mesh_renderer.hpp"
#include "game_object.hpp"
#include "color.h"

class MeshObject:public GameObject
{
public:    
    glm::quat Quat;
    // Constructor(s)
    MeshObject();
    MeshObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, Color color = WHITE, glm::vec3 velocity = glm::vec3(0.0f, 0.0f,0.0f), glm::vec3 rotationVelocity = glm::vec3(0.0f, 0.0f,0.0f));
    // Draw mesh
    void Draw(MeshRenderer &renderer);
    
    void Update(GLfloat dt);
};


#endif /* mesh_object_hpp */
