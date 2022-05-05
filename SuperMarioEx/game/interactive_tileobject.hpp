//
//  interactive_tileobject.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#ifndef interactive_tileobject_hpp
#define interactive_tileobject_hpp

#include <box2d/box2d.h>
#include "b2debugdraw.h"
#include "global.hpp"
#include "b2player.hpp"
#include "tmx.h"
#include "map_layer.hpp"
#include "tilemap_helper.hpp"

class InteractiveTileObject{
public:
    InteractiveTileObject(b2World *world, tmx_map *map, EnvItem *item);
    virtual void onHeadHit(B2Player &player)=0;
    void setCategoryFilter(short filterBit);
    MapLayer::Cell& getCell();
    
protected:
    b2World *world;;
    tmx_map *map;
    b2Body *body;
    b2Fixture *fixture;
    EnvItem *item;
};

#endif /* interactive_tileobject_hpp */
