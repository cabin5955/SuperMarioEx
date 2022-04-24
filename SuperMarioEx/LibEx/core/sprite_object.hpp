//
//  sprite_object.hpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/10/8.
//

#ifndef sprite_object_hpp
#define sprite_object_hpp


#include "glad.h"
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"
#include "game_object.hpp"
#include "mesh_renderer.hpp"

class SpriteObject:public GameObject
{
public:
    // Constructor(s)
    SpriteObject();
    SpriteObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, Color color = WHITE,
                 glm::vec2 velocity = glm::vec2(0.0f, 0.0f),
                 GLfloat rotation=0.0f);
    // Draw sprite
    void Draw(SpriteRenderer &renderer);
};

#endif /* sprite_object_hpp */
