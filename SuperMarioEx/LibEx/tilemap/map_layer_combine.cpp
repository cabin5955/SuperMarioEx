//
//  map_layer_combine.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/27.
//

#include "map_layer_combine.hpp"
#include "resource_manager.hpp"
#include "global.hpp"

MapLayerCombine::MapLayerCombine(Shader &shader, Texture2D &texture, tmx_map *map, tmx_layer *layer){
    
    this->shader = shader;
    this->texture = texture;
    this->map_width = map->width * map->tile_width;
    this->map_height = map->height * map->tile_height;
    
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Global::ScreenWidth),
                                        static_cast<GLfloat>(Global::ScreenHeight),0.0f,
                                        -1.0f, 1.0f);
    shader.use();
    shader.setMat4("view", glm::mat4(1.0));
    shader.setMat4("projection", projection);
    
    unsigned long i, j;
    unsigned int gid, x, y, w, h;
    
    tmx_tileset *ts;
    tmx_image *im;
    void* image;
    int unit_size = VERTICES_PER_SPRITE * INDICES_PER_SPRITE;
    GLfloat *vertices = (GLfloat*)malloc(map->width * map->height * unit_size * sizeof(GLfloat));
    int tileCount = 0;
    
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
                float dx = j*ts->tile_width;
                float dy = i*ts->tile_height;
                float vert_tl_x = dx/this->map_width;
                float vert_tl_y = (dy + w)/this->map_height;
                float vert_bl_x = dx/this->map_width;
                float vert_bl_y = dy/this->map_height;
                float vert_br_x = (dx + w)/this->map_width;
                float vert_br_y = dy/this->map_height;
                float vert_tr_x = (dx + w)/this->map_width;
                float vert_tr_y = (dy + h)/this->map_height;
                
                float texWidth = (float)this->texture.Width;
                float texHeight = (float)this->texture.Height;
                float tex_tl_x = x/texWidth;
                float tex_tl_y = (y + h)/texHeight;
                float tex_bl_x = x/texWidth;
                float tex_bl_y = y/texHeight;
                float tex_br_x = (x + w)/texWidth;
                float tex_br_y = y/texHeight;
                float tex_tr_x = (x + w)/texWidth;
                float tex_tr_y = (y + h)/texHeight;
                
                GLfloat a_vertice[] = {
                    // Pos                // Tex
                    vert_tl_x, vert_tl_y, tex_tl_x, tex_tl_y,
                    vert_br_x, vert_br_y, tex_br_x, tex_br_y,
                    vert_bl_x, vert_bl_y, tex_bl_x, tex_bl_y,

                    vert_tl_x, vert_tl_y, tex_tl_x, tex_tl_y,
                    vert_tr_x, vert_tr_y, tex_tr_x, tex_tr_y,
                    vert_br_x, vert_br_y, tex_br_x, tex_br_y
                };
                memcpy(&vertices[tileCount * unit_size], a_vertice, unit_size * sizeof(GLfloat));
                tileCount++;
            }
        }
    }
    
    this->verticsSizel = tileCount * 6;
    
    GLuint VBO;
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, tileCount * unit_size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    free(vertices);
}

MapLayerCombine::~MapLayerCombine()
{
    
}

//public
void MapLayerCombine::draw(glm::vec2 position)
{
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * this->map_width, 0.5f * this->map_height, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * this->map_width, -0.5f * this->map_height, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(this->map_width, this->map_height, 1.0f)); // Last scale

    this->shader.setMat4("model", model);
    // Render textured quad
    this->shader.setVec3("spriteColor", {1.0f,1.0f,1.0f});

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, this->verticsSizel);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}
