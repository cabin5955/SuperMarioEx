//
//  b2unicycle_player.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/10.
//

#include "b2unicycle_player.hpp"
#include "global.hpp"
#include "b2player.hpp"

B2UnicyclePlayer::B2UnicyclePlayer(b2World *world, glm::vec2 pos) : Player(pos){
    this->world = world;
    this->hitTimes = 0;
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->Position.x/PPM,(Global::ScreenHeight-this->Position.y)/PPM);
    bodyDef.userData.pointer = (uintptr_t)this;
    this->body = this->world->CreateBody(&bodyDef);
    this->body->SetFixedRotation(true);
    
    b2PolygonShape bodyShape;
    b2Vec2 size(16,16);
    bodyShape.SetAsBox(size.x/2.0f/PPM, size.y/2.0f/PPM);
    
    b2FixtureDef fdef;
    //for body
    fdef.filter.categoryBits = Player::NOTHING_BIT;
    fdef.filter.maskBits = Player::Player::COIN_BIT |
            Player::BRICK_BIT |
            Player::ENEMY_BIT |
            Player::ENEMY_HEAD_BIT |
            Player::ITEM_BIT;
    fdef.shape = &bodyShape;
    this->body->CreateFixture(&fdef);
    
    b2CircleShape circleShape;
    circleShape.m_radius = size.x/2.0f/PPM;
    //for unicycle
    fdef.filter.categoryBits = Player::MARIO_BIT;
    fdef.filter.maskBits = Player::GROUND_BIT |
            Player::COIN_BIT |
            Player::BRICK_BIT |
            Player::ENEMY_BIT |
            Player::OBJECT_BIT |
            Player::ENEMY_HEAD_BIT |
            Player::ITEM_BIT;
    
    fdef.density = 1;
    fdef.friction = 0.8f;
    fdef.shape = &circleShape;
    
    b2BodyDef unicycleBodyDef;
    unicycleBodyDef.type = b2_dynamicBody;
    unicycleBodyDef.position.Set( this->Position.x/PPM,(Global::ScreenHeight-this->Position.y)/PPM);
    wheel = world->CreateBody( &unicycleBodyDef );
    wheel->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;;
    
    b2RevoluteJointDef revoluteJointDef;
    revoluteJointDef.bodyA = body;
    revoluteJointDef.bodyB = wheel;
    revoluteJointDef.collideConnected = false;
    revoluteJointDef.localAnchorA.Set(0,-size.y/2.0f/PPM);//the top right corner of the box
    revoluteJointDef.localAnchorB.Set(0,0);//center of the circle
    revoluteJointDef.enableMotor = true;
    revoluteJointDef.maxMotorTorque = 10;
    revoluteJointDef.motorSpeed = 0;
    joint = (b2RevoluteJoint*)world->CreateJoint( &revoluteJointDef );
    //for head
    fdef.filter.categoryBits = Player::MARIO_BIT;
    fdef.filter.maskBits = Player::GROUND_BIT |
            Player::COIN_BIT |
            Player::BRICK_BIT |
            Player::ENEMY_BIT |
            Player::OBJECT_BIT |
            Player::ENEMY_HEAD_BIT |
            Player::ITEM_BIT;
    
    b2PolygonShape head;
    head.SetAsBox(8/2.0f/PPM, 2/2.0f/PPM , {0,10 / PPM}, 0 );
    fdef.filter.categoryBits = Player::MARIO_HEAD_BIT;
    fdef.shape = &head;
    fdef.isSensor = true;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
}

