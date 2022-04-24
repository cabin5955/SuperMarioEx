//
//  game_object.cpp
//  LearnOpenGL
//
//  Created by chen caibin on 2021/9/25.
//

#include "game_object.hpp"
GameObject::GameObject(): Position(0, 0, 0), Size(1, 1, 1), Velocity(0.0f), color(WHITE),
                        RotationAxis(0.0f,0.0f,1.0f), Sprite(), IsSolid(false),
                        Destroyed(false),RotationAngle(0.0f)
{ }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, Color c,
                       glm::vec3 velocity,glm::vec3 rotationAxis,float angle)
    : Position(pos), Size(size), Velocity(velocity), color(c), RotationAxis(rotationAxis),
      RotationAngle(angle),Sprite(sprite), IsSolid(false), Destroyed(false) { }
