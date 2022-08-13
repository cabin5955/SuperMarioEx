//
//  basic_mesh.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/7/9.
//

#include "basic_mesh.hpp"
#include <string>

BasicMesh::BasicMesh(Shader &shader, BasicVertex *vertices, unsigned int vsize, int *indices,unsigned int isize, bool calcNormals){
    this->shader = shader;
    int vertexSize = sizeof(BasicVertex)*vsize;
    int indiceSize = sizeof(int)*isize;
    this->vertices = (BasicVertex*)malloc(vertexSize);
    this->indices = (int*)malloc(indiceSize);
    memcpy(this->vertices, vertices, vertexSize);
    memcpy(this->indices, indices, indiceSize);
    
    shader.use();
    this->InitMeshData();
    this->AddVertices(this->vertices, vsize, this->indices, isize, calcNormals);
}

void BasicMesh::InitMeshData(){
    
    this->size = 0;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ibo);
}

void BasicMesh::AddVertices(BasicVertex *vertices, unsigned int vsize, int *indices, unsigned int isize, bool calcNormals)
{
    if(calcNormals)
    {
        this->CalcNormals(vertices, vsize, indices, isize);
    }
    this->size = isize;
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(BasicVertex), vertices,GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (const void*)offsetof(BasicVertex,pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (const void*)offsetof(BasicVertex,texCoord));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (const void*)offsetof(BasicVertex,normal));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

void BasicMesh::CalcNormals(BasicVertex *vertices, unsigned int vsize, int *indices,unsigned int isize)
{
    for(int i = 0; i < isize; i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];
        
        glm::vec3 v1 = vertices[i1].pos - vertices[i0].pos;
        glm::vec3 v2 = vertices[i2].pos - vertices[i0].pos;
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
        
        vertices[i0].normal = normal;
        vertices[i1].normal = normal;
        vertices[i2].normal = normal;
    }
}

void BasicMesh::Draw(Texture2D &texture, glm::vec3 position, glm::vec3 size, float rotation, glm::vec3 axis)
{
    // Prepare transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    if(rotation != 0){
        model = glm::rotate(model, rotation, axis);
    }
    model = glm::scale(model, size); // Last scale
    this->shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glBindVertexArray(vao);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES, this->size, GL_UNSIGNED_INT, nullptr);
    glDisable(GL_BLEND);
    glBindVertexArray(0);
}
