//
//  goomba.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/15.
//

#include "goomba.hpp"
#include "resource_manager.hpp"
#include "global.hpp"

Goomba::Goomba(b2World *world, glm::vec2 pos):
walk_ani(ResourceManager::GetTexture("goomba"), true),
die_ani(ResourceManager::GetTexture("goomba"), true),
Enemy(world, pos){
    this->RotationAngle = 0;
    this->Size = {16.0f,16.0f};
    this->stateTime = 0;
    this->destroyed = false;
    for (int i=0; i<2;i++) {
        walk_ani.addFrame({i,0}, this->Size);
    }
    die_ani.addFrame({2,0}, this->Size);
    this->DefineEnemy();
}

void Goomba::DefineEnemy(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->Position.x/PPM,(Global::ScreenHeight-this->Position.y)/PPM);
    bodyDef.userData.pointer = (uintptr_t)this;
    this->body = this->world->CreateBody(&bodyDef);
    this->body->SetFixedRotation(true);

    b2FixtureDef fdef;
    fdef.filter.categoryBits = Player::ENEMY_BIT;
    fdef.filter.maskBits = Player::GROUND_BIT |
            Player::COIN_BIT |
            Player::BRICK_BIT |
            Player::ENEMY_BIT |
            Player::OBJECT_BIT |
            Player::MARIO_BIT|
            Player::PLATFORM_BIT;
    
    b2PolygonShape shape;
    shape.SetAsBox(16/2.0f/PPM, 12/2.0f/PPM);
    
    fdef.shape = &shape;
    fdef.density = 1.0f;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;

    b2PolygonShape head;
    head.SetAsBox(10/2.0f/PPM, 2/2.0f/PPM , {0,6 / PPM}, 0 );
    fdef.filter.categoryBits = Player::ENEMY_HEAD_BIT;
    fdef.shape = &head;
    fdef.restitution = 0.5f;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
}

void Goomba::Update(float dt){
    if(destroyed || autoDestroyed)return;
    
    if(setToAutoDestroyed){
        autoDestroyed = true;
        world->DestroyBody(body);
        return;
    }
    
    float delta = Global::GetFrameTime();
    timerAnimation += delta * 6 /*Frames por segundo*/;
    this->body->SetLinearVelocity(velocity);
    
    stateTime += dt;
    if(setToDestroy && !destroyed){
        world->DestroyBody(body);
        destroyed = true;
        stateTime = 0;
    }else{
        walk_ani.SetTimer(timerAnimation);
        b2Vec2 pos = body->GetPosition();
        this->Position = {pos.x*PPM-0.5f*this->Size.x,Global::ScreenHeight-pos.y*PPM-0.6f*this->Size.y};
    }
}

void Goomba::Draw(SpriteRenderer &renderer)
{
    if(autoDestroyed)return;
    
    if(!destroyed || stateTime < 1){
        SubTexture2D textureRegion = walk_ani.GetCurrentFrame();
        if(setToDestroy){
            textureRegion = die_ani.GetCurrentFrame();
        }
        renderer.DrawSprite(textureRegion, this->Position, this->Size, this->flipX);
    }
}

void Goomba::HitOnHead(const B2Player &player){
    setToDestroy = true;
}

void Goomba::HitByEnemy(const Enemy &enemy){
    
}
