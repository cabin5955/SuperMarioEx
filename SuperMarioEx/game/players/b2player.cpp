//
//  b2player.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#include "b2player.hpp"
#include "global.hpp"

B2Player::B2Player(b2World *world, glm::vec2 pos) : Player(pos){
    
    this->world = world;
    this->hitTimes = 0;
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->Position.x/PPM,(Global::ScreenHeight-this->Position.y)/PPM);
    bodyDef.userData.pointer = (uintptr_t)this;
    this->body = this->world->CreateBody(&bodyDef);
    this->body->SetFixedRotation(true);
    
    b2FixtureDef fdef;
    fdef.filter.categoryBits = Player::MARIO_BIT;
    fdef.filter.maskBits = Player::GROUND_BIT |
            Player::COIN_BIT |
            Player::BRICK_BIT |
            Player::ENEMY_BIT |
            Player::OBJECT_BIT |
            Player::ENEMY_HEAD_BIT |
            Player::ITEM_BIT;
    
    b2CircleShape shape;
    shape.m_radius = 8 / PPM;
    shape.m_p = {0.0f,-2 / PPM};
//    b2PolygonShape shape;
//    shape.SetAsBox(8/PPM, 8/PPM , {0,0}, 0 );
    fdef.shape = &shape;
    fdef.density = 1.0f;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
    
    b2PolygonShape head;
    head.SetAsBox(8/2.0f/PPM, 2/2.0f/PPM , {0, 8/PPM}, 0 );
    fdef.filter.categoryBits = Player::MARIO_HEAD_BIT;
    fdef.shape = &head;
    fdef.isSensor = true;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
}

