//
//  map2body_builder.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/10.
//

#include "map2body_builder.hpp"

void Map2BodyBuilder::BuildEnvItemObject(tmx_map *map, b2World *world, EnvItem *item)
{
    glm::vec2 pos = {item->position.x,item->position.y};
    glm::vec2 size = {item->size.x,item->size.y};
    b2BodyDef bd;
    int map_y = map->height-(pos.y+size.y)/map->tile_height;
    int py = map_y * map->tile_height + size.y/2.0f;
    float base = Global::ScreenHeight - (map->height)*map->tile_height;
    bd.position.Set((pos.x+size.x/2.0f)/PPM, (py+base)/PPM);
    b2Body* body = world->CreateBody(&bd);

    b2PolygonShape shape;
    shape.SetAsBox(size.x/2.0f/PPM, size.y/2.0f/PPM);
    
    b2FixtureDef fdef;
    fdef.filter.categoryBits = Player::OBJECT_BIT;
    fdef.filter.maskBits = Player::GROUND_BIT |
            Player::COIN_BIT |
            Player::BRICK_BIT |
            Player::ENEMY_BIT |
            Player::OBJECT_BIT |
            Player::MARIO_BIT;
    
    fdef.shape = &shape;
    body->CreateFixture(&fdef);
}

void Map2BodyBuilder::BuildAllPolylineObjects(b2World *world)
{
    for (int i = 0; i < TilemapHelper::polyLineItems.size(); i++)
    {
        MultPointsItem *item = &TilemapHelper::polyLineItems[i];
        if(item->vertices.size()<3){
            b2BodyDef bd;
            b2Body* edge = world->CreateBody(&bd);
            b2EdgeShape shape;
            for(int j = 1; j < item->vertices.size();j++ ){
                auto width = item->vertices[j].x - item->vertices[j-1].x;
                b2Vec2 v0((item->vertices[j-1].x-width)/PPM, (Global::ScreenHeight-item->vertices[j-1].y)/PPM);
                b2Vec2 v1( item->vertices[j-1].x/PPM, (Global::ScreenHeight-item->vertices[j-1].y)/PPM);
                b2Vec2 v2( item->vertices[j].x/PPM, (Global::ScreenHeight-item->vertices[j].y)/PPM);
                b2Vec2 v3((item->vertices[j].x+width)/PPM, (Global::ScreenHeight-item->vertices[j].y)/PPM);
                shape.SetOneSided(v0, v1, v2, v3);
            }
            b2FixtureDef fdef;
            fdef.filter.categoryBits = Player::OBJECT_BIT;
            fdef.friction = 1.0f;
            fdef.density = 0.0f;
            fdef.shape = &shape;
            edge->CreateFixture(&fdef);
        }
    }
}

void Map2BodyBuilder::BuildAllPolygonObjects(b2World *world)
{
    for (int i = 0; i < TilemapHelper::polygonItems.size(); i++)
    {
        MultPointsItem *item = &TilemapHelper::polygonItems[i];
        b2BodyDef bd;
        b2Body* polygon = world->CreateBody(&bd);
        b2Vec2 vertices[8];
        int size = (int)item->vertices.size();
        for(int j = 0; j < size;j++ ){
            vertices[j].Set(item->vertices[j].x/PPM, (Global::ScreenHeight-item->vertices[j].y)/PPM);
        }
        b2PolygonShape shape;
        shape.Set(vertices, size);
        b2FixtureDef fdef;
        fdef.filter.categoryBits = Player::OBJECT_BIT;
        fdef.friction = 1.0f;
        fdef.density = 0.0f;
        fdef.shape = &shape;
        polygon->CreateFixture(&fdef);
    }
}
