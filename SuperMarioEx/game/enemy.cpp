//
//  enemy.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/9.
//

#include "enemy.hpp"
Enemy::Enemy(b2World *world, glm::vec2 pos){
    this->world = world;
    this->Position = pos;
}
