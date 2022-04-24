
#ifndef map_layer_hpp
#define map_layer_hpp

#include <vector>
#include "glad.h"
#include "tmx.h"
#include "shader.h"
#include "texture.hpp"

#define VERTICES_PER_SPRITE  4          // Vertices Per Sprite
#define INDICES_PER_SPRITE  6           // Indices Per Sprite

class MapLayer final
{
public:
    MapLayer(Shader &shader, Texture2D &texture, tmx_map *map, tmx_layer *layer);
    
    ~MapLayer();
    
    MapLayer(const MapLayer&) = delete;
    MapLayer& operator = (const MapLayer&) = delete;

    void draw(glm::vec2 position);

private:
    
    GLuint quadVAO;
    Shader shader;
    Texture2D texture;
    
    float map_width,map_height;
    unsigned int verticsSizel;
};

#endif /* map_layer_hpp */
