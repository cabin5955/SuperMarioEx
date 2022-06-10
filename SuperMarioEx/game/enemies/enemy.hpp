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
    bool autoDestroyed;
    bool setToAutoDestroyed;
    
public:
    glm::vec2  Position, Size;
    Color       color;
    float       RotationAngle;
    
    float timerAnimation;
    b2Vec2 velocity;
    bool flipX;
    b2Body* body;
    
    Enemy(b2World *world, glm::vec2 pos);
    
    virtual void Update(float dt) = 0;
    virtual void HitOnHead(const B2Player &player) = 0;
    virtual void HitByEnemy(const Enemy &enemy) = 0;
    virtual void Draw(SpriteRenderer &renderer) = 0;
    
    void ReverseVelocity(bool x, bool y);
    void DropDownDestroyed();
};


#endif /* enemy_hpp */
