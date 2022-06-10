//
//  oposum.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/9.
//

#include "oposum.hpp"
#include "resource_manager.hpp"
#include "global.hpp"

Oposum::Oposum(b2World *world, glm::vec2 pos)
:walk_ani(ResourceManager::GetTexture("oposum"), true),Enemy(world, pos){
    this->RotationAngle = 0;
    this->Size = {36.0f,28.0f};
    for (int i=0; i<6;i++) {
        walk_ani.addFrame({i,0}, this->Size);
    }
    this->DefineEnemy();
}

void Oposum::DefineEnemy(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->Position.x/PPM,(Global::ScreenHeight-this->Position.y)/PPM);
    bodyDef.userData.pointer = (uintptr_t)this;
    this->body = this->world->CreateBody(&bodyDef);
    this->body->SetFixedRotation(true);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = B2Player::ENEMY_BIT;
    fdef.filter.maskBits = B2Player::GROUND_BIT |
            B2Player::COIN_BIT |
            B2Player::BRICK_BIT |
            B2Player::ENEMY_BIT |
            B2Player::OBJECT_BIT |
            B2Player::MARIO_BIT|
            B2Player::PLATFORM_BIT;
    
    b2PolygonShape shape;
    shape.SetAsBox(24/2.0f/PPM, 12/2.0f/PPM);
    
    fdef.shape = &shape;
    fdef.density = 1.0f;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;

    b2PolygonShape head;
    head.SetAsBox(20/2.0f/PPM, 2/2.0f/PPM , {0,6 / PPM}, 0 );
    fdef.filter.categoryBits = B2Player::ENEMY_HEAD_BIT;
    fdef.shape = &head;
    fdef.isSensor = true;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
}

void Oposum::Update(float dt){
    if(autoDestroyed) return;
    
    float delta = Global::GetFrameTime();
    timerAnimation += delta * 6 /*Frames por segundo*/;
    walk_ani.SetTimer(timerAnimation);
}

void Oposum::Draw(SpriteRenderer &renderer)
{
    if(autoDestroyed) return;
    
    SubTexture2D textureRegion = walk_ani.GetCurrentFrame();
    renderer.DrawSprite(textureRegion, this->Position, this->Size, this->flipX);
}

void Oposum::HitOnHead(const B2Player &player){
    
}

void Oposum::HitByEnemy(const Enemy &enemy){
    
}
