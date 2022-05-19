//
//  world_contact_listener.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/3.
//

#include "world_contact_listener.hpp"
#include "b2player.hpp"
#include "interactive_tileobject.hpp"
#include "enemy.hpp"

void WorldContactListener::BeginContact(b2Contact* contact) {
    b2Fixture *fixA = contact->GetFixtureA();
    b2Fixture *fixB = contact->GetFixtureB();

    int cDef = fixA->GetFilterData().categoryBits | fixB->GetFilterData().categoryBits;
    switch (cDef)
    {
        case B2Player::MARIO_HEAD_BIT | B2Player::BRICK_BIT:
            
        case B2Player::MARIO_HEAD_BIT | B2Player::COIN_BIT:
            break;
            
        case B2Player::ENEMY_HEAD_BIT | B2Player::MARIO_BIT:
            if(fixA->GetFilterData().categoryBits == B2Player::ENEMY_HEAD_BIT){
                Enemy* enemy = (Enemy*)fixA->GetUserData().pointer;
                enemy->HitOnHead(*((B2Player*)fixB->GetUserData().pointer));
            }
            else{
                Enemy* enemy = (Enemy*)fixB->GetUserData().pointer;
                enemy->HitOnHead(*((B2Player*)fixA->GetUserData().pointer));
            }
            break;
            
        case B2Player::ENEMY_BIT | B2Player::OBJECT_BIT:
            if(fixA->GetFilterData().categoryBits == B2Player::ENEMY_BIT){
                Enemy* enemy = (Enemy*)fixA->GetUserData().pointer;
                enemy->ReverseVelocity(true, false);
            }
            else{
                Enemy* enemy = (Enemy*)fixB->GetUserData().pointer;
                enemy->ReverseVelocity(true, false);
            }
            break;
            
        case B2Player::MARIO_BIT | B2Player::ENEMY_BIT:
            
            break;
            
        case B2Player::ENEMY_BIT | B2Player::ENEMY_BIT:
        {
            Enemy* enemyA = (Enemy*)fixA->GetUserData().pointer;
            enemyA->ReverseVelocity(true, false);
            Enemy* enemyB = (Enemy*)fixB->GetUserData().pointer;
            enemyB->ReverseVelocity(true, false);
            break;
        }
        case B2Player::ITEM_BIT | B2Player::OBJECT_BIT:
            
            break;
            
        case B2Player::ITEM_BIT | B2Player::MARIO_BIT:
            
            break;
            
        case B2Player::FIREBALL_BIT | B2Player::OBJECT_BIT:
            
            break;
            
        case B2Player::ENEMY_BIT | B2Player::PLATFORM_BIT:
            if(fixA->GetFilterData().categoryBits == B2Player::ENEMY_BIT){
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
        case B2Player::MARIO_HEAD_BIT | B2Player::BRICK_BIT:
            
        case B2Player::MARIO_HEAD_BIT | B2Player::COIN_BIT:
            if(fixA->GetFilterData().categoryBits == B2Player::MARIO_HEAD_BIT){
                InteractiveTileObject* userData = (InteractiveTileObject*)fixB->GetUserData().pointer;
                userData->onHeadHit(*((B2Player*)fixA->GetUserData().pointer));
            }
            else{
                InteractiveTileObject* userData = (InteractiveTileObject*)fixA->GetUserData().pointer;
                userData->onHeadHit(*((B2Player*)fixB->GetUserData().pointer));
            }
            break;
            
        case B2Player::ENEMY_HEAD_BIT | B2Player::MARIO_BIT:
            
            break;
            
        case B2Player::ENEMY_BIT | B2Player::OBJECT_BIT:
            break;
            
        case B2Player::MARIO_BIT | B2Player::ENEMY_BIT:
            
            break;
            
        case B2Player::ENEMY_BIT | B2Player::ENEMY_BIT:
        
            break;
    
        case B2Player::ITEM_BIT | B2Player::OBJECT_BIT:
            
            break;
            
        case B2Player::ITEM_BIT | B2Player::MARIO_BIT:
            
            break;
            
        case B2Player::FIREBALL_BIT | B2Player::OBJECT_BIT:
            
            break;
    }
}
