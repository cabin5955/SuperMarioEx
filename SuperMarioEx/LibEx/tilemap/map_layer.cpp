
#include "map_layer.hpp"
#include "resource_manager.hpp"
#include "global.hpp"

MapLayer::MapLayer(Shader &shader, Texture2D &texture, const tmx_map *map, const tmx_layer *layer){
    
    static int indexCount = 0;
    index = indexCount++;
    this->map = map;
    this->layer = layer;
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
    
    onAttach();
    genCells();
    genVertex();
}

void MapLayer::genCells(){
    unsigned long i, j;
    unsigned int gid;
    
    for (i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            gid = (layer->content.gids[(i*map->width)+j]) & TMX_FLIP_BITS_REMOVAL;
            Cell cell;
            cell.layerIndex = index;
            cell.tile = NULL;
            if (map->tiles[gid] != NULL) {
                Tile *tile = new Tile();
                tile->ts = map->tiles[gid]->tileset;
                tile->im = map->tiles[gid]->image;
                tile->x  = map->tiles[gid]->ul_x;
                tile->y  = map->tiles[gid]->ul_y;
                cell.tile = tile;
            }
            this->cells.push_back(cell);
        }
    }
}

unsigned int MapLayer::getIndex(){
    return index;
}

void MapLayer::genVertex(){
    unsigned long i, j;
    unsigned int x, y, w, h;
    tmx_tileset *ts;
    tmx_image *im;
    
    indexCount = 0;
    vertexArr.clear();
    for (i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            Cell cell = this->cells[(i*map->width)+j];
            if (cell.tile != NULL) {
                ts = cell.tile->ts;
                im = cell.tile->im;
                x  = cell.tile->x;
                y  = cell.tile->y;
                w  = ts->tile_width;
                h  = ts->tile_height;
                
//                if (im) {
//                    image = im->resource_image;
//                }
//                else {
//                    image = ts->image->resource_image;
//                }
                
                float dx = j*ts->tile_width;
                float dy = i*ts->tile_height;
                
                float vert_tl_x = dx;
                float vert_tl_y = (dy + w);
                float vert_bl_x = dx;
                float vert_bl_y = dy;
                float vert_br_x = (dx + w);
                float vert_br_y = dy;
                float vert_tr_x = (dx + w);
                float vert_tr_y = (dy + h);
                
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
                
                T_Vertex v0;
                v0.position = {vert_tl_x, vert_tl_y, 0.0f};
                v0.color = {1.0f, 1.0f, 1.0f, 1.0f};
                v0.texCoords = {tex_tl_x, tex_tl_y};
                v0.texIndex = this->texture.ID;
                vertexArr.push_back(v0);
                
                T_Vertex v1;
                v1.position = {vert_tr_x, vert_tr_y, 0.0f};
                v1.color = {1.0f, 1.0f, 1.0f, 1.0f};
                v1.texCoords = {tex_tr_x, tex_tr_y};
                v1.texIndex = this->texture.ID;
                vertexArr.push_back(v1);
                
                T_Vertex v2;
                v2.position = {vert_br_x, vert_br_y, 0.0f};
                v2.color = {1.0f, 1.0f, 1.0f, 1.0f};
                v2.texCoords = {tex_br_x, tex_br_y};
                v2.texIndex = this->texture.ID;
                vertexArr.push_back(v2);
                
                T_Vertex v3;
                v3.position = {vert_bl_x, vert_bl_y, 0.0f};
                v3.color = {1.0f, 1.0f, 1.0f, 1.0f};
                v3.texCoords = {tex_bl_x, tex_bl_y};
                v3.texIndex = this->texture.ID;
                vertexArr.push_back(v3);
                
                indexCount += 6;
            }
        }
    }
}

void MapLayer::onAttach(){
    const size_t MaxQuadCount = 10000;
    const size_t MaxVertexCount = MaxQuadCount * 4;
    const size_t MaxIndexCount = MaxQuadCount * 6;
    
    glGenVertexArrays(1,&m_QuadVA);
    glBindVertexArray(m_QuadVA);
    
    glGenBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(T_Vertex), nullptr, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T_Vertex), (const void*)offsetof(T_Vertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(T_Vertex), (const void*)offsetof(T_Vertex,color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(T_Vertex), (const void*)offsetof(T_Vertex,texCoords));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 1, GL_INT, sizeof(T_Vertex), (const void*)offsetof(T_Vertex,texIndex));
    glEnableVertexAttribArray(3);
    
    uint32_t* indices = new uint32_t[MaxIndexCount];
    uint32_t offset = 0;
    for (size_t i = 0; i < MaxIndexCount; i += 6)
    {
        indices[i] = 0 + offset;
        indices[i + 1] = 2 + offset;
        indices[i + 2] = 3 + offset;

        indices[i + 3] = 0 + offset;
        indices[i + 4] = 1 + offset;
        indices[i + 5] = 2 + offset;

        offset += 4;
    }
    
    glGenBuffers(1, &m_QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    delete[] indices;
}

MapLayer::Cell& MapLayer::getCell(unsigned int x,unsigned int y){
    return this->cells[(y*map->width)+x];
}

void MapLayer::draw(glm::vec2 position)
{
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    this->shader.setMat4("model", model);
    // Render textured quad
    this->shader.setVec3("spriteColor", {1.0f,1.0f,1.0f});

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexArr.size() * sizeof(T_Vertex), vertexArr.data());
    glBindVertexArray(m_QuadVA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}
