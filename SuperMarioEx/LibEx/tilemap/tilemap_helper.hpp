//
//  tilemap_utils.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/12.
//

#ifndef tilemap_helper_hpp
#define tilemap_helper_hpp

#include <vector>
#include <string>
#include "stb_image.h"
#include "tmx.h"
#include "glad.h"
#include "scene.h"
#include "map_layer.hpp"
#include "tile.hpp"
#include "resource_manager.hpp"
#include "player.hpp"

class TilemapHelper {
    
public:
    static void* ex_tex_loader(const char *path);
    static void ex_free_tex(void *ptr);
    static void get_tilemap_objects(const char *name, tmx_object_group *objgr);
    static void get_tilemap_tile(void *image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
                                 unsigned int dx, unsigned int dy, float opacity, unsigned int flags);

    static void get_tilemap_image_layer(tmx_image *image);
    static void get_tilemap_layer(tmx_map *map, tmx_layer *layer);
    static void* get_first_tile_image(tmx_map *map, tmx_layer *layer);
    static void get_tilemap_all_layers(tmx_map *map, tmx_layer *layers);
    
    static std::vector<Tile*> tiles;
    static std::vector<MapLayer*> mapLayers;
    static std::vector<EnvItem> gameItems;
    static std::vector<MultPointsItem> polyLineItems;
    static std::vector<MultPointsItem> polygonItems;
};

#endif /* tilemap_helper_hpp */
