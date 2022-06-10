//
//  b2unicycle_player.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/10.
//

#ifndef b2unicycle_player_hpp
#define b2unicycle_player_hpp

#include "player.hpp"
#include <box2d/box2d.h>
#include "b2debugdraw.h"

class B2UnicyclePlayer:public Player {
private:
    b2World* world;

public:
    b2Body* body;
    b2Body* wheel;
    b2RevoluteJoint* joint;
    
    B2UnicyclePlayer(b2World *world, glm::vec2 pos);
};

#endif /* b2unicycle_player_hpp */
