//
//  b2player.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#ifndef b2player_hpp
#define b2player_hpp

#include "player.hpp"
#include <box2d/box2d.h>
#include "b2debugdraw.h"

class B2Player:public Player {
private:
    b2World* world;

public:
    
    static const short NOTHING_BIT = 0;
    static const short GROUND_BIT = 1;
    static const short MARIO_BIT = 2;
    static const short BRICK_BIT = 4;
    static const short COIN_BIT = 8;
    static const short DESTROYED_BIT = 16;
    static const short OBJECT_BIT = 32;
    static const short ENEMY_BIT = 64;
    static const short ENEMY_HEAD_BIT = 128;
    static const short ITEM_BIT = 256;
    static const short MARIO_HEAD_BIT = 512;
    static const short FIREBALL_BIT = 1024;
    b2Body* body;
    
    B2Player(b2World *world, glm::vec2 pos);
};

#endif /* b2player_hpp */
