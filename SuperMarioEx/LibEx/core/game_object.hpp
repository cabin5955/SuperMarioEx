//
//  game_object.hpp
//  LearnOpenGL
//
//  Created by chen caibin on 2021/9/25.
//

#ifndef game_object_hpp
#define game_object_hpp

#include "glad.h"
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"
#include "mesh_renderer.hpp"
#include "color.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // Object state
    glm::vec3   Position, Size, Velocity;
    glm::vec3   RotationAxis;
    Color       color;
    float       RotationAngle;
    GLboolean   IsSolid;
    GLboolean   Destroyed;
    // Render state
    Texture2D   Sprite;
    // Constructor(s)
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, Color color = WHITE,
               glm::vec3 velocity = glm::vec3(0.0f, 0.0f,0.0f),
               glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f,1.0f),
               float angle=0.0f);
    
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer){};
    
    // Draw mesh
    virtual void Draw(MeshRenderer &renderer){};
};

#endif /* game_object_hpp */
