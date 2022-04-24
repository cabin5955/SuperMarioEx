//
//  color_renderer.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/1/11.
//

#include "color_renderer.hpp"
ColorRenderer::ColorRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

ColorRenderer::~ColorRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void ColorRenderer::DrawColor(Color color, glm::vec2 position, glm::vec2 size, GLfloat rotate)
{
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);;
    model = glm::translate(model, glm::vec3(position, 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
    this->shader.setMat4("model", model);

    // Render textured quad
    this->shader.setVec4("color", {color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f});

    glBindVertexArray(this->quadVAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}

void ColorRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
