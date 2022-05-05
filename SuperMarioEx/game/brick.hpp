//
//  brick.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#ifndef brick_hpp
#define brick_hpp

#include "interactive_tileobject.hpp"

class Brick:public InteractiveTileObject{
public:
    Brick(b2World *world, tmx_map *map, EnvItem *item);
    void onHeadHit(B2Player &player);
};

#endif /* brick_hpp */
