//
//  mesh_object.cpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/10/8.
//

#include "mesh_object.hpp"
MeshObject::MeshObject(): GameObject() { }

MeshObject::MeshObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, Color color, glm::vec3 velocity, glm::vec3 rotationVelocity)
    : GameObject(pos,size,sprite,color,velocity,rotationVelocity),Quat(1.f,0.0f,0.0f,0.0f) {    }

void MeshObject::Draw(MeshRenderer &renderer)
{
    //rotation way 1
    renderer.DrawMesh(this->Sprite, this->Position, this->Size, this->Quat, this->color);
    
    //rotation way 2
    //renderer.DrawMesh(this->Sprite, this->Position, this->Size, this->RotationAxis, this->Axis, this->Color);
}

void MeshObject::Update(GLfloat dt)
{
    this->Position += this->Velocity * dt;
    //this->RotationAxis = this->RotationVelocity * dt;
    
    glm::quat q = glm::rotate(this->Quat, this->RotationAxis.x, glm::vec3(1.0f,0.0f,0.0f));
    q = glm::rotate(q, this->RotationAxis.y, glm::vec3(0.0f,1.0f,0.0f));
    this->Quat = glm::rotate(q, this->RotationAxis.z, glm::vec3(0.0f,0.0f,1.0f));
}
