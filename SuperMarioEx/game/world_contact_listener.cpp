//
//  world_contact_listener.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/3.
//

#include "world_contact_listener.hpp"
#include "player.hpp"
#include "interactive_tileobject.hpp"
#include "enemy.hpp"

void WorldContactListener::BeginContact(b2Contact* contact) {
    b2Fixture *fixA = contact->GetFixtureA();
    b2Fixture *fixB = contact->GetFixtureB();

    int cDef = fixA->GetFilterData().categoryBits | fixB->GetFilterData().categoryBits;
    switch (cDef)
    {
        case Player::MARIO_HEAD_BIT | Player::BRICK_BIT:
            
        case Player::MARIO_HEAD_BIT | Player::COIN_BIT:
            break;
            
        case Player::ENEMY_HEAD_BIT | Player::MARIO_BIT:
            if(fixA->GetFilterData().categoryBits == Player::ENEMY_HEAD_BIT){
                Enemy* enemy = (Enemy*)fixA->GetUserData().pointer;
                enemy->HitOnHead(*((B2Player*)fixB->GetUserData().pointer));
            }
            else{
                Enemy* enemy = (Enemy*)fixB->GetUserData().pointer;
                enemy->HitOnHead(*((B2Player*)fixA->GetUserData().pointer));
            }
            break;
            
        case Player::ENEMY_BIT | Player::OBJECT_BIT:
            if(fixA->GetFilterData().categoryBits == Player::ENEMY_BIT){
                Enemy* enemy = (Enemy*)fixA->GetUserData().pointer;
                enemy->ReverseVelocity(true, false);
            }
            else{
                Enemy* enemy = (Enemy*)fixB->GetUserData().pointer;
                enemy->ReverseVelocity(true, false);
            }
            break;
            
        case Player::MARIO_BIT | Player::ENEMY_BIT:
            
            break;
            
        case Player::MARIO_BIT | Player::OBJECT_BIT:
        {
            Player *player = NULL;
            if(fixA->GetFilterData().categoryBits == Player::MARIO_BIT){
                player = (Player*)fixA->GetUserData().pointer;
            }
            else{
                player = (Player*)fixB->GetUserData().pointer;
            }
            player->hitTimes++;
            if(player->hitTimes > 0){
                player->canJump = true;
                player->isJump = false;
            }
            //printf("player hit object! times %d\n",player->hitTimes);
        }
            break;
            
        case Player::ENEMY_BIT | Player::ENEMY_BIT:
        {
            Enemy* enemyA = (Enemy*)fixA->GetUserData().pointer;
            enemyA->ReverseVelocity(true, false);
            Enemy* enemyB = (Enemy*)fixB->GetUserData().pointer;
            enemyB->ReverseVelocity(true, false);
            break;
        }
        case Player::ITEM_BIT | Player::OBJECT_BIT:
            
            break;
            
        case Player::ITEM_BIT | Player::MARIO_BIT:
            
            break;
            
        case Player::FIREBALL_BIT | Player::OBJECT_BIT:
            
            break;
            
        case Player::ENEMY_BIT | Player::PLATFORM_BIT:
            if(fixA->GetFilterData().categoryBits == Player::ENEMY_BIT){
                Enemy* enemy = (Enemy*)fixA->GetUserData().pointer;
                enemy->DropDownDestroyed();
                
            }else{
                Enemy* enemy = (Enemy*)fixB->GetUserData().pointer;
                enemy->DropDownDestroyed();
            }
            break;
    }
}

void WorldContactListener::EndContact(b2Contact* contact) {
      
    b2Fixture *fixA = contact->GetFixtureA();
    b2Fixture *fixB = contact->GetFixtureB();

    int cDef = fixA->GetFilterData().categoryBits | fixB->GetFilterData().categoryBits;

    switch (cDef)
    {
        case Player::MARIO_HEAD_BIT | Player::BRICK_BIT:
            
        case Player::MARIO_HEAD_BIT | Player::COIN_BIT:
            if(fixA->GetFilterData().categoryBits == Player::MARIO_HEAD_BIT){
                InteractiveTileObject* userData = (InteractiveTileObject*)fixB->GetUserData().pointer;
                userData->onHeadHit(*((B2Player*)fixA->GetUserData().pointer));
            }
            else{
                InteractiveTileObject* userData = (InteractiveTileObject*)fixA->GetUserData().pointer;
                userData->onHeadHit(*((B2Player*)fixB->GetUserData().pointer));
            }
            break;
            
        case Player::ENEMY_HEAD_BIT | Player::MARIO_BIT:
            
            break;
            
        case Player::ENEMY_BIT | Player::OBJECT_BIT:
            break;
            
        case Player::MARIO_BIT | Player::ENEMY_BIT:
            
            break;
            
        case Player::MARIO_BIT | Player::OBJECT_BIT:
        {
            Player *player = NULL;
            if(fixA->GetFilterData().categoryBits == Player::MARIO_BIT){
                player = (Player*)fixA->GetUserData().pointer;
            }
            else{
                player = (Player*)fixB->GetUserData().pointer;
            }
            player->hitTimes--;
            if(player->hitTimes == 0){
                player->canJump = false;
                player->isJump = true;
            }
            //printf("player leave object! times %d\n",player->hitTimes);
        }
            break;
            
        case Player::ENEMY_BIT | Player::ENEMY_BIT:
        
            break;
    
        case Player::ITEM_BIT | Player::OBJECT_BIT:
            
            break;
            
        case Player::ITEM_BIT | Player::MARIO_BIT:
            
            break;
            
        case Player::FIREBALL_BIT | Player::OBJECT_BIT:
            
            break;
    }
}
