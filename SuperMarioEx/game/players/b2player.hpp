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
    b2Body* body;
    
    B2Player(b2World *world, glm::vec2 pos);
};

#endif /* b2player_hpp */
