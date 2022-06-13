//
//  b2car.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/11.
//

#ifndef b2car_hpp
#define b2car_hpp

#include <box2d/box2d.h>
#include "b2debugdraw.h"
#include "player.hpp"

class B2Car:public GameObject {
private:
    b2World* world;

public:
    bool isWalk;
    bool isJump;
    float speed;
    bool canJump;
    int hitTimes;
    
    b2Body* body;
    b2Body* wheel1;
    b2Body* wheel2;
    b2WheelJoint* spring1;
    b2WheelJoint* spring2;
    
    B2Car(b2World *world, glm::vec2 pos);
    void Update();
    void Draw(SpriteRenderer &renderer) override;
    void Draw(SpriteRenderer &renderer, glm::vec2 &pos);
};

#endif /* b2car_hpp */
