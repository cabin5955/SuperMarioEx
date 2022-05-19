//
//  sprite_renderer.cpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/9/25.
//

#include "sprite_renderer.hpp"
#include "utils.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    //this->InitDrawArrays();
    this->InitDrawElements();
}

SpriteRenderer::~SpriteRenderer()
{
    //glDeleteVertexArrays(1, &this->quadVAO);
    glDeleteVertexArrays(1, &this->m_QuadVA);
    glDeleteBuffers(1, &this->m_QuadVB);
    glDeleteBuffers(1, &this->m_QuadIB);
}

void SpriteRenderer::SetFlipX(bool flip){
    this->shader.use();
    if(flip)
        this->shader.setInt("flipx", 1);
    else
        this->shader.setInt("flipx", 0);
}

void SpriteRenderer::SetFlipY(bool flip){
    this->shader.use();
    if(flip)
        this->shader.setInt("flipy", 1);
    else
        this->shader.setInt("flipy", 0);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, glm::vec3 axis,GLfloat rotate, Color color)
{
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, axis); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

    this->shader.setMat4("model", model);

    // Render textured quad
    this->shader.setVec3("spriteColor", {color.r/255.0f,color.g/255.0f,color.b/255.0f});

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

//    glBindVertexArray(this->quadVAO);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glDisable(GL_BLEND);
//    glBindVertexArray(0);
    
    Vertex vertices[] = {
        {{0.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}}
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex), vertices);
    glBindVertexArray(m_QuadVA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(SubTexture2D &texture, glm::vec2 position, glm::vec2 size, bool flipX, glm::vec3 axis,GLfloat rotate, Color color)
{
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, axis); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

    this->shader.setMat4("model", model);

    // Render textured quad
    this->shader.setVec3("spriteColor", {color.r/255.0f,color.g/255.0f,color.b/255.0f});

    glActiveTexture(GL_TEXTURE0);
    texture.GetTexture().Bind();
    
    const glm::vec2* texCoords = texture.GetTexCoords();
    glm::vec2 texCoordArr[4];
    if(flipX){
        texCoordArr[0] = texCoords[1];
        texCoordArr[1] = texCoords[0];
        texCoordArr[2] = texCoords[3];
        texCoordArr[3] = texCoords[2];
    }
    else{
        for(int i = 0;i < 4;i++)
            texCoordArr[i] = texCoords[i];
    }
        
    
    Vertex vertices[] = {
        {{0.0f, 1.0f}, texCoordArr[3]},
        {{1.0f, 1.0f}, texCoordArr[2]},
        {{1.0f, 0.0f}, texCoordArr[1]},
        {{0.0f, 0.0f}, texCoordArr[0]}
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex), vertices);
    glBindVertexArray(m_QuadVA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawTile(Tile &tile){
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tile.position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * tile.size.x, 0.5f * tile.size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * tile.size.x, -0.5f * tile.size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(tile.size, 1.0f)); // Last scale

    this->shader.setMat4("model", model);

    // Render textured quad
    this->shader.setVec3("spriteColor", {1.0f,1.0f,1.0f});

    glActiveTexture(GL_TEXTURE0);
    tile.texture->Bind();

//    glBindVertexArray(tile.quadVAO);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glDisable(GL_BLEND);
//    glBindVertexArray(0);
    
    Vertex vertices[] = {
        {{0.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}}
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex), vertices);
    glBindVertexArray(m_QuadVA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void SpriteRenderer::InitDrawArrays()
{
    // Configure VAO/VBO
    GLuint VBO;
    Vertex vertices[] = {
        // Pos      // Tex
        {{0.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.0f}, {0.0f, 0.0f}},

        {{0.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f}},
        {{1.0f, 0.0f}, {1.0f, 0.0f}}
    };
    
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,texCoords));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::InitDrawElements(){
    
    const size_t VertexCount = 4;
    const size_t IndexCount = 6;
    
    glGenVertexArrays(1,&m_QuadVA);
    glBindVertexArray(m_QuadVA);
    
    glGenBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, VertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,texCoords));
    glEnableVertexAttribArray(1);
    
    uint32_t* indices = new uint32_t[IndexCount];
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 3;

    indices[3] = 0;
    indices[4] = 1;
    indices[5] = 2;
    
    glGenBuffers(1, &m_QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    delete[] indices;
}
