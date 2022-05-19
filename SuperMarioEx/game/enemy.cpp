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
    this->velocity = {-0.5f, -1.0f};
    this->setToAutoDestroyed = false;
    this->autoDestroyed = false;
}

void Enemy::ReverseVelocity(bool x, bool y){
    if(x)
        velocity.x = -velocity.x;
    if(y)
        velocity.y = -velocity.y;
}

void Enemy::DropDownDestroyed(){
    setToAutoDestroyed = true;
    printf("set to auto destroyed!\n");
}
