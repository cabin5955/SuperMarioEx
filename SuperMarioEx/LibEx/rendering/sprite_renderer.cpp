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
    this->InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
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

    glBindVertexArray(this->quadVAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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

    glBindVertexArray(tile.quadVAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

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
