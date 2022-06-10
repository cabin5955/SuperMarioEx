//
//  player.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/4.
//

#include "player.hpp"
#include "resource_manager.hpp"
#include "global.hpp"

Player::Player(glm::vec2 pos) :
climb_ani("player-climb", 3, true),
crouch_ani("player-crouch", 2, false),
hurt_ani("player-hurt", 2, false),
idle_ani("player-idle", 4, true),
jump_ani("player-jump", 2, false),
run_ani("player-run", 6, true)
{
    climb_ani.Preload();
    crouch_ani.Preload();
    hurt_ani.Preload();
    idle_ani.Preload();
    jump_ani.Preload();
    run_ani.Preload();

    this->Sprite = ResourceManager::GetTexture("player-idle-1.png");
    this->Position = {pos.x, pos.y, 0};
    this->RotationAxis = {0,0,1};
    this->Size = {this->Sprite.Width,this->Sprite.Height,1.0f};
    
    this->isWalk = false;
    this->flipX = false;
    this->timerAnimation = 0.0;
}

void Player::Update(EnvItem *envItems, int envItemsLength)
{
    float delta = Global::GetFrameTime();
    timerAnimation += delta * 9 /*Frames por segundo*/;
    
    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        glm::vec3 *p = &(this->Position);
        if (ei->blocking &&
            ei->position.x <= p->x &&
            ei->position.x + ei->size.x >= p->x &&
            ei->position.y >= p->y &&
            ei->position.y < p->y + this->speed*delta)
        {
            hitObstacle = 1;
            this->speed = 0.0f;
            p->y = ei->position.y;
            this->isJump = false;
        }
    }

    if (!hitObstacle)
    {
        this->Position.y += this->speed*delta;
        this->speed += G*delta;
        this->canJump = false;
    }
    else {
        this->canJump = true;
    }
    std::string name;
    if(isJump)
    {
        jump_ani.SetTimer(timerAnimation);
        name = jump_ani.GetCurrentFrame();
        this->Sprite = ResourceManager::GetTexture(name);
    }
    else if(isWalk)
    {
        run_ani.SetTimer(timerAnimation);
        name = run_ani.GetCurrentFrame();
        this->Sprite = ResourceManager::GetTexture(name);
    }
    else
    {
        idle_ani.SetTimer(timerAnimation);
        name = idle_ani.GetCurrentFrame();
        this->Sprite = ResourceManager::GetTexture(name);
    }
}

void Player::Update()
{
    float delta = Global::GetFrameTime();
    std::string name;
    timerAnimation += delta * 9 /*Frames por segundo*/;
    if(isJump)
    {
        jump_ani.SetTimer(timerAnimation);
        name = jump_ani.GetCurrentFrame();
        this->Sprite = ResourceManager::GetTexture(name);
    }
    else if(isWalk)
    {
        run_ani.SetTimer(timerAnimation);
        name = run_ani.GetCurrentFrame();
        this->Sprite = ResourceManager::GetTexture(name);
    }
    else
    {
        idle_ani.SetTimer(timerAnimation);
        name = idle_ani.GetCurrentFrame();
        this->Sprite = ResourceManager::GetTexture(name);
    }
}

void Player::Draw(SpriteRenderer &renderer)
{
    glm::vec3 pos = {this->Position.x,this->Position.y-this->Size.y,this->Position.z};
    renderer.SetFlipX(this->flipX);
    renderer.DrawSprite(this->Sprite, pos, this->Size, this->RotationAxis, this->RotationAngle, this->color);
    renderer.SetFlipX(false);
}

void Player::Draw(SpriteRenderer &renderer, glm::vec2 &pos)
{
    renderer.SetFlipX(this->flipX);
    renderer.DrawSprite(this->Sprite, pos, this->Size, this->RotationAxis, this->RotationAngle, this->color);
    renderer.SetFlipX(false);
}
