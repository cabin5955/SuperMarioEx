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
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->Position.x/PPM,(Global::ScreenHeight-this->Position.y)/PPM);
    bodyDef.userData.pointer = (uintptr_t)this;
    this->body = this->world->CreateBody(&bodyDef);
    this->body->SetFixedRotation(true);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = B2Player::MARIO_BIT;
    fdef.filter.maskBits = B2Player::GROUND_BIT |
            B2Player::COIN_BIT |
            B2Player::BRICK_BIT |
            B2Player::ENEMY_BIT |
            B2Player::OBJECT_BIT |
            B2Player::ENEMY_HEAD_BIT |
            B2Player::ITEM_BIT;
    b2PolygonShape shape;
    shape.SetAsBox(12/2.0f/PPM, 20/2.0f/PPM);
//    b2CircleShape shape;
//    shape.m_radius = 10 / PPM;
//    shape.m_p = {0.0f,-2 / PPM};
    fdef.shape = &shape;
    fdef.density = 1.0f;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
    
//    b2EdgeShape head ;
//    head.m_vertex1 = {-2 / PPM, 16 / PPM};
//    head.m_vertex2 = {2 / PPM, 16 / PPM};
    b2PolygonShape head;
    head.SetAsBox(12/2.0f/PPM, 2/2.0f/PPM , {0,10 / PPM}, 0 );
    fdef.filter.categoryBits = B2Player::MARIO_HEAD_BIT;
    fdef.shape = &head;
    fdef.isSensor = true;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
    
}
