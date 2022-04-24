//
//  sprite_object.cpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/10/8.
//

#include "sprite_object.hpp"
SpriteObject::SpriteObject(): GameObject()
{ }

SpriteObject::SpriteObject(glm::vec2 pos, glm::vec2 size,
                           Texture2D sprite, Color color,
                           glm::vec2 velocity,
                           GLfloat rotation)
:GameObject(glm::vec3(pos.x,pos.y,1.0f),
            glm::vec3(size.x,size.y,1.0f),
            sprite,color,
            glm::vec3(velocity.x,velocity.y,0.0f),
            glm::vec3(0.0f,0.0f,1.0f),
            rotation)
                                                          
{
    
}

void SpriteObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->RotationAxis, this->RotationAngle, this->color);
}
