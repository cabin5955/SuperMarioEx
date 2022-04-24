//
//  tile.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/3/30.
//

#include "tile.hpp"
Tile::Tile(Texture2D *texture, Rectangle source, Rectangle dest)
{
    this->texture = texture;
    float width = (float)texture->Width;
    float height = (float)texture->Height;

    this->position.x = dest.x;
    this->position.y = dest.y;
    this->size.x = dest.width;
    this->size.y = dest.height;
    
    bool flipX = false;

    if (source.width < 0) {
        flipX = true; source.width *= -1;
    }
    if (source.height < 0) {
        source.y -= source.height;
    }
    float tex_tl_x,tex_tl_y;
    float tex_bl_x,tex_bl_y;
    float tex_br_x,tex_br_y;
    float tex_tr_x,tex_tr_y;
    
    if(flipX){
        tex_tl_x = (source.x + source.width)/width;
        tex_tl_y = source.y/height;
    }else{
//        tex_tl_x = source.x/width;
//        tex_tl_y = source.y/height;
        tex_tl_x = source.x/width;
        tex_tl_y = (source.y + source.height)/height;
    }
    
    if(flipX){
        tex_bl_x = (source.x + source.width)/width;
        tex_bl_y = (source.y + source.height)/height;
    }else{
//        tex_bl_x = source.x/width;
//        tex_bl_y = (source.y + source.height)/height;
        tex_bl_x = source.x/width;
        tex_bl_y = source.y/height;
    }
    
    if(flipX){
        tex_br_x = source.x/width;
        tex_br_y = (source.y + source.height)/height;
    }else{
//        tex_br_x = (source.x + source.width)/width;
//        tex_br_y = (source.y + source.height)/height;
        tex_br_x = (source.x + source.width)/width;
        tex_br_y = source.y/height;
    }
    
    if(flipX){
        tex_tr_x = source.x/width;
        tex_tr_y = source.y/height;
    }else{
//        tex_tr_x = (source.x + source.width)/width;
//        tex_tr_y = source.y/height;
        tex_tr_x = (source.x + source.width)/width;
        tex_tr_y = (source.y + source.height)/height;
    }
    
    GLfloat vertices[] = {
        // Pos                // Tex
        0.0f, 1.0f, tex_tl_x, tex_tl_y,
        1.0f, 0.0f, tex_br_x, tex_br_y,
        0.0f, 0.0f, tex_bl_x, tex_bl_y,

        0.0f, 1.0f, tex_tl_x, tex_tl_y,
        1.0f, 1.0f, tex_tr_x, tex_tr_y,
        1.0f, 0.0f, tex_br_x, tex_br_y
    };
    GLuint VBO;
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Tile::~Tile()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}
