//
//  tmx_loader.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/3/26.
//

#ifndef tmx_loader_hpp
#define tmx_loader_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "texture.hpp"

// for collision shapes a polygon with n points
typedef struct Poly
{
    int           numPoints;
    glm::vec2*    points;
} Poly;

// an array of polygons
typedef struct PolyArrays
{
    int         numPolys;
    Poly*       polys;
} PolyArrays;

// a spritesheet (or tile map)
typedef struct SpriteSheet
{
     Texture2D      texture;
     glm::vec2      frameSize;
     int            framesWide;
     glm::vec2      origin;
     int            firstGid;
     int            tileCount;
     PolyArrays*    colShapes; // one per tile
} SpriteSheet;

// a layer of the map
typedef struct Layer {
    int** cells;
} Layer;

// freely positioned sprites (y-sorted with player)
typedef struct TObject
{
    glm::vec2   pos;
    int         gid;
    int         sorty;      // translated cartesian Y (to iso coord)
} TObject;

// container for whole map
typedef struct Tmx
{
    int tileWidth;
    int tileHeight;             // map tile size (layers can be different)
    int mapWidth;
    int mapHeight;              // map size in tiles, all layers are this size (really!)

    int numTsets;               // number of tile sheets
    SpriteSheet** spriteSheets; // array of pointers to sprite sheet pointers
    int numLayers;              // count of the layers
    Layer** layers;             // pointer to an array of layer pointers
    
    int numObj;                 // count of objects
    TObject* objects;            // objects or freely placable sprites
} Tmx;

class TmxLoader{
    
public:
   static SpriteSheet* createSpriteSheet(const char* textureName,
                                         glm::vec2 frameSize,
                                    int framesWide, glm::vec2 origin);
    
    static void freeSpriteSheet(SpriteSheet* ss);
    
    static Tmx* LoadTmx(const char* filename);

    // free the resources used by the tmx
    static void freeTmx(Tmx* map);
    
};

#endif /* tmx_loader_hpp */
