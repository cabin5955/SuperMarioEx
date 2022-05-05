
#ifndef map_layer_hpp
#define map_layer_hpp

#include <vector>
#include "glad.h"
#include "tmx.h"
#include "shader.h"
#include "texture.hpp"
#include "vertex.h"

#define VERTICES_PER_SPRITE  4          // Vertices Per Sprite
#define INDICES_PER_SPRITE  6           // Indices Per Sprite

class MapLayer final
{
public:
    
    typedef struct {
        unsigned int x;
        unsigned int y;
        tmx_tileset *ts;
        tmx_image *im;
    }Tile;
    
    typedef struct {
        unsigned int layerIndex;
        Tile *tile;
    }Cell;
    
    MapLayer(Shader &shader, Texture2D &texture, const tmx_map *map, const tmx_layer *layer);
    
    ~MapLayer();
    
    MapLayer(const MapLayer&) = delete;
    MapLayer& operator = (const MapLayer&) = delete;

    Cell& getCell(unsigned int x,unsigned int y);
    unsigned int getIndex();
    void genVertex();
    void draw(glm::vec2 position);
    void drawElements(glm::vec2 position);
    std::vector<Cell> cells;

private:
    
    Shader shader;
    Texture2D texture;
    GLuint m_QuadVA;
    GLuint m_QuadVB;
    GLuint m_QuadIB;
    
    const tmx_map *map;
    const tmx_layer *layer;
    unsigned int index;
    float map_width,map_height;
    std::vector<T_Vertex> vertexArr;
    unsigned int indexCount;
    
    void onAttach();
    void genCells();
};

#endif /* map_layer_hpp */
