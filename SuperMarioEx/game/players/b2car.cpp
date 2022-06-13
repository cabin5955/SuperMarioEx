//
//  b2car.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/11.
//

#include "b2car.hpp"

#include "resource_manager.hpp"
#include "global.hpp"

B2Car::B2Car(b2World *world, glm::vec2 pos){
    
    this->Sprite = ResourceManager::GetTexture("player-idle-1.png");
    this->Position = {pos.x, pos.y, 0};
    this->RotationAxis = {0,0,1};
    this->Size = {this->Sprite.Width,this->Sprite.Height,1.0f};
    
    this->isWalk = false;
    this->world = world;
    this->hitTimes = 0;
    
    float scale = 0.2f;
    b2PolygonShape chassis;
    b2Vec2 vertices[8];
    vertices[0].Set(-1.5f, -0.5f);
    vertices[1].Set(1.5f, -0.5f);
    vertices[2].Set(1.5f, 0.0f);
    vertices[3].Set(0.0f, 0.9f);
    vertices[4].Set(-1.15f, 0.9f);
    vertices[5].Set(-1.5f, 0.2f);
    for (int i=0; i<6; i++) {
        vertices[i] *= scale;
    }
    chassis.Set(vertices, 6);

    b2CircleShape circle;
    circle.m_radius = 0.4f * scale;

    float bx = this->Position.x/PPM;
    float by = (Global::ScreenHeight-this->Position.y)/PPM;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(bx,by);
    this->body = world->CreateBody(&bd);
    this->body->CreateFixture(&chassis, 1.0f);

    b2FixtureDef fd;
    fd.shape = &circle;
    fd.density = 1.0f;
    fd.friction = 0.9f;

    bd.position.Set(bx-1.0f * scale, by-(1.0f-0.35f) * scale);
    this->wheel1 = this->world->CreateBody(&bd);
    this->wheel1->CreateFixture(&fd);

    bd.position.Set(bx+1.0f * scale, by-(1.0f-0.4f) * scale);
    this->wheel2 = this->world->CreateBody(&bd);
    this->wheel2->CreateFixture(&fd);

    b2WheelJointDef jd;
    b2Vec2 axis(0.0f, 1.0f);

    float mass1 = this->wheel1->GetMass();
    float mass2 = this->wheel2->GetMass();

    float hertz = 4.0f;
    float dampingRatio = 0.7f;
    float omega = 2.0f * b2_pi * hertz;

    jd.Initialize(this->body, this->wheel1, this->wheel1->GetPosition(), axis);
    jd.motorSpeed = 0.0f;
    jd.maxMotorTorque = 10.0f;
    jd.enableMotor = true;
    jd.stiffness = mass1 * omega * omega;
    jd.damping = 2.0f * mass1 * dampingRatio * omega * scale;
    jd.lowerTranslation = -0.25f * scale;
    jd.upperTranslation = 0.25f * scale;
    jd.enableLimit = true;
    this->spring1 = (b2WheelJoint*)this->world->CreateJoint(&jd);

    jd.Initialize(this->body, this->wheel2, this->wheel2->GetPosition(), axis);
    jd.motorSpeed = 0.0f;
    jd.maxMotorTorque = 5.0f;
    jd.enableMotor = false;
    jd.stiffness = mass2 * omega * omega;
    jd.damping = 2.0f * mass2 * dampingRatio * omega * scale;
    jd.lowerTranslation = -0.25f * scale;
    jd.upperTranslation = 0.25f * scale;
    jd.enableLimit = true;
    this->spring2 = (b2WheelJoint*)this->world->CreateJoint(&jd);
}

void B2Car::Update()
{
    
}

void B2Car::Draw(SpriteRenderer &renderer)
{
    //glm::vec3 pos = {this->Position.x,this->Position.y-this->Size.y,this->Position.z};
    //renderer.DrawSprite(this->Sprite, pos, this->Size, this->RotationAxis, this->RotationAngle, this->color);
}

void B2Car::Draw(SpriteRenderer &renderer, glm::vec2 &pos)
{
    //renderer.DrawSprite(this->Sprite, pos, this->Size, this->RotationAxis, this->RotationAngle, this->color);
}
