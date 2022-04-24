//
//  MeshRenderer.cpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/10/7.
//

#include "mesh_renderer.hpp"
#include "global.hpp"
#include "objloader.hpp"

MeshRenderer::MeshRenderer(Shader &shader,float *vertics,size_t vSize,
                           float *texcoords,size_t tSize,float *normals,size_t nSize)
{
    this->shader = shader;
    Init(vertics,vSize, texcoords, tSize, normals, nSize);
}

MeshRenderer::MeshRenderer(Shader &shader,
             std::vector<glm::vec3> & vertices,
             std::vector<glm::vec2> & uvs,
             std::vector<glm::vec3> & normals)
{
    this->shader = shader;
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    
    glGenVertexArrays(1, &this->meshVAO);
    glBindVertexArray(this->meshVAO);
    
    glGenBuffers(1, &this->vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &this->normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &this->uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        1,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 3rd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        2,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
                          );
    this->verticsSize = (int)vertices.size();
}

MeshRenderer::~MeshRenderer()
{
    glDeleteVertexArrays(1, &this->meshVAO);
}

void MeshRenderer::Init(float *vertics, size_t vSize, float *texcoords, size_t tSize, float *normals, size_t nSize)
{
    unsigned int shipVBO;
    glGenVertexArrays(1, &this->meshVAO);
    glGenBuffers(1, &shipVBO);
    glBindVertexArray(this->meshVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, shipVBO);
    glBufferData(GL_ARRAY_BUFFER,vSize+nSize+tSize,nullptr,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vertics);
    glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, normals);
    glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize, tSize, texcoords);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)vSize);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float),
                          (GLvoid*)(vSize+nSize));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    this->verticsSize = (GLuint)vSize/(sizeof(float)*3);
}

void MeshRenderer::DrawMesh(Texture2D &texture, glm::vec3 position,
              glm::vec3 size, glm::quat q, Color color)
{
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
    model *= glm::mat4_cast(q);
    model = glm::scale(model, size); // Last scale

    this->shader.setMat4("model", model);
    this->shader.setVec4("meshColor", {color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f});
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    //glBindVertexArray(this->meshVAO);
    
    glDrawArrays(GL_TRIANGLES, 0, this->verticsSize);
    
    //glBindVertexArray(0);
}

void MeshRenderer::DrawMesh(Texture2D &texture, glm::vec3 position, glm::vec3 size,
                            GLfloat rotation, glm::vec3 axis, Color color)
{
//    glm::quat quat = glm::angleAxis(rotation, axis);
//    this->DrawMesh(texture, position, size, quat, color);
    
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);;
    model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

    //model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, rotation, axis); // Then rotate
    //model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

    model = glm::scale(model, size); // Last scale

    this->shader.setMat4("model", model);
    this->shader.setVec4("meshColor", {color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f});
    
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, this->verticsSize);
    glBindVertexArray(0);
}

void MeshRenderer::DrawMesh(Shader &shader,Model &model,Texture2D &texture,
              glm::vec3 position,glm::vec3 size,GLfloat rotation, glm::vec3 axis, Color color)
{
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    
    // Prepare transformations
    shader.use();
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, position);
    //modelMat *= glm::mat4_cast(q);
    modelMat = glm::rotate(modelMat, rotation, axis); // Then rotate
    modelMat = glm::scale(modelMat, size); // Last scale

    shader.setMat4("model", modelMat);
    shader.setVec4("meshColor", {color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f});
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(model.meshVAO);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, model.vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, model.normalbuffer);
    glVertexAttribPointer(
        1,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 3rd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, model.uvbuffer);
    glVertexAttribPointer(
        2,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
                          );
    glDrawArrays(GL_TRIANGLES, 0, model.verticsSize);
    
    glBindVertexArray(0);
}
