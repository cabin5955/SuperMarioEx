//
//  coin.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#include "coin.hpp"

Coin::Coin(b2World *world, tmx_map *map, EnvItem *item):InteractiveTileObject(world, map, item){
    this->fixture->GetUserData().pointer = (uintptr_t)this;
    setCategoryFilter(B2Player::COIN_BIT);
}

void Coin::onHeadHit(B2Player &player){
    printf("on head hit coin!\n");
    const int BLANK_COIN = 28;
    if(!hitted){
        hitted = true;
        MapLayer::Cell &cell = getCell();
        int layer = cell.layerIndex;
        cell.tile->ts = map->tiles[BLANK_COIN]->tileset;
        cell.tile->im = map->tiles[BLANK_COIN]->image;
        cell.tile->x  = map->tiles[BLANK_COIN]->ul_x;
        cell.tile->y  = map->tiles[BLANK_COIN]->ul_y;
        TilemapHelper::mapLayers[layer]->genVertex();
    }
}
