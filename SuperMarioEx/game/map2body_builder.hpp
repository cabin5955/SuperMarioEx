//
//  map2body_builder.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/10.
//

#ifndef map2body_builder_hpp
#define map2body_builder_hpp

#include "tmx.h"
#include "map_layer.hpp"
#include "tile.hpp"
#include "player.hpp"
#include "tilemap_helper.hpp"
#include <box2d/box2d.h>
#include "global.hpp"

class Map2BodyBuilder{
public:
    static void BuildEnvItemObject(tmx_map *map,b2World *world,EnvItem *item);
    static void BuildAllPolylineObjects(b2World *world);
    static void BuildAllPolygonObjects(b2World *world);
};

#endif /* map2body_builder_hpp */
