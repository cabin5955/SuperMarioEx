//
//  tilemap_helper.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/12.
//

#include "tilemap_helper.hpp"

std::vector<Tile*> TilemapHelper::tiles;
std::vector<MapLayer*> TilemapHelper::mapLayers;
std::vector<EnvItem> TilemapHelper::gameItems;
std::vector<MultPointsItem> TilemapHelper::polyLineItems;
std::vector<MultPointsItem> TilemapHelper::polygonItems;

void* TilemapHelper::ex_tex_loader(const char *path) {
    //printf("tile map tex path %s \n",path);
    Texture2D *texture = new Texture2D();
    texture->Internal_Format = GL_RGBA;
    texture->Image_Format = GL_RGBA;
    
    // Load image
    int width, height, nrComponents;
    stbi_convert_iphone_png_to_rgb(1);
    unsigned char* image = stbi_load(path, &width, &height, &nrComponents, 0);
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    else
        format = GL_RED;
    
    texture->Internal_Format = format;
    texture->Image_Format = format;
    
    // Now generate texture
    texture->Generate(width, height, image);
    // And finally free image data
    stbi_image_free(image);
    return texture;
}

void TilemapHelper::ex_free_tex(void *ptr)
{
    free(ptr);
}

void TilemapHelper::get_tilemap_objects(const char *name, tmx_object_group *objgr) {
    
    tmx_object *head = objgr->head;
    
    while (head) {
        if (head->visible) {
            if (head->obj_type == OT_SQUARE) {
                printf("object square(%f,%f) order %d \n",head->x,head->y,objgr->draworder);
                Color clr = PINK;
                clr.a *= 0.5f;
                EnvItem item = {{head->x,head->y},{head->width, head->height},4,clr};
                strcpy(item.name, name);
                gameItems.push_back(item);
            }
            else if (head->obj_type  == OT_POLYGON) {
                printf("object OT_POLYGON,len %d\n",head->content.shape->points_len);
                MultPointsItem pItem;
                int i;
                for (i=0; i<head->content.shape->points_len; i++) {
                    auto px = head->x+head->content.shape->points[i][0];
                    auto py = head->y+head->content.shape->points[i][1];
                    pItem.vertices.push_back({px,py});
                }
                polygonItems.push_back(pItem);
            }
            else if (head->obj_type == OT_POLYLINE) {
                printf("object OT_POLYLINE, points len %d \n",head->content.shape->points_len);
                MultPointsItem plItem;
                int i;
                for (i=0; i<head->content.shape->points_len; i++) {
                    auto px = head->x+head->content.shape->points[i][0];
                    auto py = head->y+head->content.shape->points[i][1];
                    plItem.vertices.push_back({px,py});
                }
                polyLineItems.push_back(plItem);
            }
            else if (head->obj_type == OT_ELLIPSE) {
                printf("object OT_ELLIPSE\n");
            }
        }
        head = head->next;
    }
    printf("object square num %d \n",(int)gameItems.size());
}

void TilemapHelper::get_tilemap_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
                      unsigned int dx, unsigned int dy, float opacity, unsigned int flags) {
    
    Tile *t = new Tile((Texture2D*)image,
    {(float)sx,(float)sy,(float)sw,(float)sh},
    {(float)dx,(float)dy,(float)sw,(float)sh});
    tiles.push_back(t);
}

void TilemapHelper::get_tilemap_layer(tmx_map *map, tmx_layer *layer) {
    unsigned long i, j;
    unsigned int gid, x, y, w, h, flags;
    float op;
    tmx_tileset *ts;
    tmx_image *im;
    void* image;
    op = layer->opacity;
    for (i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            gid = (layer->content.gids[(i*map->width)+j]) & TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                x  = map->tiles[gid]->ul_x;
                y  = map->tiles[gid]->ul_y;
                w  = ts->tile_width;
                h  = ts->tile_height;
                if (im) {
                    image = im->resource_image;
                }
                else {
                    image = ts->image->resource_image;
                }
                flags = (layer->content.gids[(i*map->width)+j]) & ~TMX_FLIP_BITS_REMOVAL;
                get_tilemap_tile(image, x, y, w, h, (unsigned int)j*ts->tile_width, (unsigned int)i*ts->tile_height, op, flags);
            }
        }
    }
}

void* TilemapHelper::get_first_tile_image(tmx_map *map, tmx_layer *layer){
    int i,j;
    unsigned int gid;
    tmx_tileset *ts;
    tmx_image *im;
    for (i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            gid = (layer->content.gids[(i*map->width)+j]) & TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                if (im) {
                    return im->resource_image;
                }
                else {
                    return ts->image->resource_image;
                }
            }
        }
    }
    return NULL;
}

void TilemapHelper::get_tilemap_image_layer(tmx_image *image) {

    printf("image layer \n");
}

void TilemapHelper::get_tilemap_all_layers(tmx_map *map, tmx_layer *layers) {
    while (layers) {
        if (layers->visible) {

            if (layers->type == L_GROUP) {
                get_tilemap_all_layers(map, layers->content.group_head);
            }
            else if (layers->type == L_OBJGR) {
                printf("L_OBJGR layer %s\n",layers->name);
                get_tilemap_objects(layers->name, layers->content.objgr);
            }
            else if (layers->type == L_IMAGE) {
                get_tilemap_image_layer(layers->content.image);
            }
            else if (layers->type == L_LAYER) {
                printf("L_LAYER layer %s\n",layers->name);
                Texture2D *tex = (Texture2D*)get_first_tile_image(map, layers);
                MapLayer *layer = new MapLayer(ResourceManager::GetShader("tilebatch"),*tex,map,layers);
                mapLayers.push_back(layer);
            }
        }
        layers = layers->next;
    }
}
