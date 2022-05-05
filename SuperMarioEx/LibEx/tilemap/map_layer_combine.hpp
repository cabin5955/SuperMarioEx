//
//  map_layer_combine.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/27.
//

#ifndef map_layer_combine_hpp
#define map_layer_combine_hpp

#include <vector>
#include "glad.h"
#include "tmx.h"
#include "shader.h"
#include "texture.hpp"
#include "vertex.h"

#define VERTICES_PER_SPRITE  4          // Vertices Per Sprite
#define INDICES_PER_SPRITE  6           // Indices Per Sprite

class MapLayerCombine final
{
public:
    MapLayerCombine(Shader &shader, Texture2D &texture, tmx_map *map, tmx_layer *layer);
    
    ~MapLayerCombine();
    
    MapLayerCombine(const MapLayerCombine&) = delete;
    MapLayerCombine& operator = (const MapLayerCombine&) = delete;

    void draw(glm::vec2 position);

private:
    
    GLuint quadVAO;
    Shader shader;
    Texture2D texture;
    
    float map_width,map_height;
    unsigned int verticsSizel;
};

#endif /* map_layer_combine_hpp */
