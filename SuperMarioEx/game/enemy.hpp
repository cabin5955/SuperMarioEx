//
//  enemy.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/9.
//

#ifndef enemy_hpp
#define enemy_hpp

#include "spritesheets_animation.hpp"
#include "game_object.hpp"
#include <box2d/box2d.h>
#include "b2player.hpp"

class Enemy {
protected:
    b2World* world;
    virtual void defineEnemy() = 0;
    
public:
    glm::vec2  Position, Size;
    Color       color;
    float       RotationAngle;
    
    float timerAnimation;
    float speed;
    bool flipX;
    b2Body* body;
    
    Enemy(b2World *world, glm::vec2 pos);
    
    virtual void update(float dt) = 0;
    virtual void hitOnHead(const B2Player &player) = 0;
    virtual void hitByEnemy(const Enemy &enemy) = 0;
    virtual void Draw(SpriteRenderer &renderer) = 0;
};


#endif /* enemy_hpp */
