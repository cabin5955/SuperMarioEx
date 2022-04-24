//
//  tile.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/3/30.
//

#ifndef tile_hpp
#define tile_hpp

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.hpp"
#include "mesh.hpp"

class Tile
{
public:
    // Constructor (inits shaders/shapes)
    Tile(Texture2D *texture, Rectangle source, Rectangle dest);
    // Destructor
    ~Tile();
    
    Texture2D *texture;
    GLuint quadVAO;
    glm::vec2 position;
    glm::vec2 size;
    
};

#endif /* tile_hpp */
