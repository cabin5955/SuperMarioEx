//
//  world_contact_listener.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/3.
//

#include "world_contact_listener.hpp"
#include "b2player.hpp"
#include "interactive_tileobject.hpp"

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
