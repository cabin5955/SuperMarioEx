//
//  interactive_tileobject.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/2.
//

#include "interactive_tileobject.hpp"

InteractiveTileObject::InteractiveTileObject(b2World *world, tmx_map *map, EnvItem *item){
    
    this->world = world;
    this->map = map;
    this->item = item;
    
    glm::vec2 pos = {item->position.x,item->position.y};
    glm::vec2 size = {item->size.x,item->size.y};
    b2BodyDef bd;
    int map_y = map->height-(pos.y+size.y)/map->tile_height;
    int py = map_y * map->tile_height + size.y/2.0f;
    float base = Global::ScreenHeight - (map->height)*map->tile_height;
    bd.position.Set((pos.x+size.x/2.0f)/PPM, (py+base)/PPM);
    this->body = world->CreateBody(&bd);

    b2PolygonShape shape;
    shape.SetAsBox(size.x/2.0f/PPM, size.y/2.0f/PPM);
    this->fixture = this->body->CreateFixture(&shape, 0.0f);
}

void InteractiveTileObject::setCategoryFilter(short filterBit){
    b2Filter filter;
    filter.categoryBits = filterBit;
    this->fixture->SetFilterData(filter);
}

MapLayer::Cell& InteractiveTileObject::getCell(){
    int x = (int)(body->GetPosition().x * PPM / 16);
    int y = (int)((Global::ScreenHeight - body->GetPosition().y * PPM) / 16);
    return TilemapHelper::mapLayers[1]->getCell(x,y);
}
