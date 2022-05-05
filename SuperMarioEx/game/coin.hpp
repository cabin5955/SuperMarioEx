//
//  Coin.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#ifndef coin_hpp
#define coin_hpp

#include "interactive_tileobject.hpp"

class Coin:public InteractiveTileObject{
public:
    Coin(b2World *world, tmx_map *map, EnvItem *item);
    void onHeadHit(B2Player &player);
    
private:
    bool hitted;
};

#endif /* coin_hpp */
