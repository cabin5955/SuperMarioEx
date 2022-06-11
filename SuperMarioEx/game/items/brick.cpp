//
//  brick.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#include "brick.hpp"

Brick::Brick(b2World *world, tmx_map *map, EnvItem *item):InteractiveTileObject(world, map, item){
    this->fixture->GetUserData().pointer = (uintptr_t)this;
    setCategoryFilter(Player::BRICK_BIT);
}

void Brick::onHeadHit(B2Player &player){
    printf("on head hit brick!\n");
    setCategoryFilter(Player::DESTROYED_BIT);
    MapLayer::Cell &cell = getCell();
    int layer = cell.layerIndex;
    cell.tile = NULL;
    TilemapHelper::mapLayers[layer]->genVertex();
}
