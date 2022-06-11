//
//  turtle.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/18.
//

#include "turtle.hpp"
#include "resource_manager.hpp"
#include "global.hpp"

Turtle::Turtle(b2World *world, glm::vec2 pos):
walk_ani(ResourceManager::GetTexture("turtle"), true),
stand_ani(ResourceManager::GetTexture("turtle"), true),
moving_ani(ResourceManager::GetTexture("turtle"), true),
Enemy(world, pos){
    this->RotationAngle = 0;
    this->Size = {16.0f,16.0f};
    this->stateTime = 0;
    this->destroyed = false;
    for (int i=0; i<2;i++) {
        walk_ani.addFrame({i,0}, this->Size);
    }
    stand_ani.addFrame({4,0}, this->Size);
    moving_ani.addFrame({4,0}, this->Size);
    currentState = previousState = WALKING;
    this->DefineEnemy();
}

void Turtle::DefineEnemy(){
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
            Player::MARIO_BIT |
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
    //fdef.isSensor = true;
    fdef.restitution = 0.5f;
    this->body->CreateFixture(&fdef)->GetUserData().pointer = (uintptr_t)this;
}

void Turtle::Update(float dt){
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
    switch (currentState){
        case MOVING_SHELL:
            moving_ani.SetTimer(timerAnimation);
            break;
            
        case STANDING_SHELL:
            stand_ani.SetTimer(timerAnimation);
            break;
            
        case WALKING:
            default:
            walk_ani.SetTimer(timerAnimation);
            break;
        }
    
    if(currentState == STANDING_SHELL && stateTime > 5){
        currentState = WALKING;
        velocity.x = 1;
        printf("WAKE UP SHELL\n");
    }
    b2Vec2 pos = body->GetPosition();
    this->Position = {pos.x*PPM-0.5f*this->Size.x,Global::ScreenHeight-pos.y*PPM-0.6f*this->Size.y};
}

void Turtle::Draw(SpriteRenderer &renderer)
{
    if(destroyed || autoDestroyed)return;
    
    SubTexture2D textureRegion = walk_ani.GetCurrentFrame();
    switch (currentState){
        case MOVING_SHELL:
            textureRegion = moving_ani.GetCurrentFrame();
            break;
            
        case STANDING_SHELL:
            textureRegion = stand_ani.GetCurrentFrame();
                    break;
            
        case WALKING:
            default:
            //textureRegion = walk_ani.GetCurrentFrame();
            break;
    }

    if(velocity.x > 0 ){
        this->flipX = true;
    }
    if(velocity.x < 0 ){
        this->flipX = false;
    }
    float dt = Global::GetFrameTime();
    stateTime = currentState == previousState ? stateTime + dt : 0;
    //update previous state
    previousState = currentState;
    
    renderer.DrawSprite(textureRegion, this->Position, this->Size, this->flipX);
}

void Turtle::HitOnHead(const B2Player &player){
    
    if(currentState == STANDING_SHELL) {
        if(player.body->GetPosition().x > this->body->GetPosition().x)
            velocity.x = -2;
        else
            velocity.x = 2;
        currentState = MOVING_SHELL;
        printf("Set to moving shell\n");
    }
    else {
        currentState = STANDING_SHELL;
        velocity.x = 0;
    }
}

void Turtle::HitByEnemy(const Enemy &enemy){
    this->ReverseVelocity(true, false);
}
