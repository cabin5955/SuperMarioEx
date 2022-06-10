//
//  game_b2platformer.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/3.
//

#include "game_b2platformer.hpp"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

GameB2Platformer* GameB2Platformer::s_instance = nullptr;

GameB2Platformer::EnvItem envItems_b2[] = {
    {{ 0, 400 }, { 1000, 400 }, 0, LIGHTGRAY },
    {{ 0, 0 }, { 1000, 400 }, 4, BROWN },
    {{ 300, 500 }, { 400, 10 }, 4, GRAY },
    {{ 250, 600 }, { 100, 10 }, 4, GRAY },
    {{ 650, 450 }, { 100, 10 }, 4, GRAY },

};

int envItemsLength_b2 = sizeof(envItems_b2)/sizeof(envItems_b2[0]);

void GoLeft_platformer()
{

}

void GoRight_platformer()
{
    
}

void Jump_platformer()
{
    
}

void UpdatePlayer(GameB2Platformer::Player *player, GameB2Platformer::EnvItem *envItems, int envItemsLength, float delta)
{
    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++)
    {
        GameB2Platformer::EnvItem *ei = envItems + i;
        glm::vec2 *p = &(player->position);
        if (ei->blocking &&
            ei->position.x <= p->x &&
            ei->position.x + ei->size.x >= p->x &&
            ei->position.y >= p->y &&
            ei->position.y < p->y + player->speed*delta)
        {
            hitObstacle = 1;
            player->speed = 0.0f;
            p->y = ei->position.y;
        }
    }

    if (!hitObstacle)
    {
        player->position.y += player->speed*delta;
        player->speed += G*delta;
        player->canJump = false;
    }
    else player->canJump = true;
}


void UpdateCameraCenter(Camera2D *camera, GameB2Platformer::Player *player, int width, int height);

void UpdateCameraCenterInsideMap(Camera2D *camera, GameB2Platformer::Player *player,
                                      GameB2Platformer::EnvItem *envItems, int envItemsLength, int width, int height);

void UpdateCameraCenterSmoothFollow(Camera2D *camera, GameB2Platformer::Player *player, float delta, int width, int height);

void GameB2Platformer::Init(unsigned int width, unsigned int height)
{
    this->Width = width;
    this->Height = height;
    
    // 配置着色器
    glm::mat4 spriteProjection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                        0.0f, static_cast<GLfloat>(this->Height),
                                            -1.0f, 1.0f);
    
    glm::mat4 uiProjection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                        static_cast<GLfloat>(this->Height),0.0f,
                                        -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("view", glm::mat4(1.0));
    spriteShader.setMat4("projection", spriteProjection);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", glm::mat4(1.0));
    colorShader.setMat4("projection", spriteProjection);
    
    Shader uiShader = ResourceManager::GetShader("ui");
    uiShader.use();
    uiShader.setMat4("projection", uiProjection);
    
    // 设置专用于渲染的控制
    spriteRenderer = new SpriteRenderer(spriteShader);
    colorRenderer = new ColorRenderer(colorShader);
    uiRenderer = new SpriteRenderer(uiShader);

    float scal = 1.0f;
    int btn_size = 64;
    GoButton_Right = new Button(this,glm::vec2(this->Width/2+btn_size*scal-btn_size*4,this->Height-128*scal),
                                glm::vec2(btn_size*scal,btn_size*scal),
                                0,
                                ResourceManager::GetTexture("arrow"),
                                GoRight_platformer,0);
    
    GoButton_Left = new Button(this,glm::vec2(this->Width/2-btn_size*scal-btn_size*4,this->Height-128*scal),
                               glm::vec2(btn_size*scal,btn_size*scal),
                               180.0f*DEG2RAD,
                               ResourceManager::GetTexture("arrow"),
                               GoLeft_platformer,0);
    
    JumpButton = new Button(this,glm::vec2(this->Width-btn_size*4,this->Height-128*scal),
                            glm::vec2(btn_size*scal,btn_size*scal),
                            -90.0f*DEG2RAD,
                            ResourceManager::GetTexture("arrow"),
                            Jump_platformer,0);
    
    player.position = { (width-200)/2.0f, 500.0f };
    player.size = {40, 40};
    player.speed = 0;
    player.canJump = true;
    
    camera.target = player.position;
    camera.offset = { width/2.0f, height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    g_debugDraw.Create();
    
    b2Vec2 gravity = b2Vec2(0.0f, -8.0f);
    world = new b2World(gravity);
    world->SetDebugDraw(&g_debugDraw);
    
    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(player.position.x/PPM,player.position.y/PPM);
    playerBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&player);;
    body = world->CreateBody(&playerBodyDef);
    body->SetFixedRotation(true);
    
    b2PolygonShape playerShape;
    playerShape.SetAsBox(player.size.x/2.0f/PPM, player.size.y/2.0f/PPM);
    body->CreateFixture(&playerShape, 1.0f);
    
    b2CircleShape circleShape;
    circleShape.m_radius = player.size.x/2.0f/PPM;
    
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1;
    fixtureDef.friction = 0.8f;
    fixtureDef.shape = &circleShape;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set( player.position.x/PPM,player.position.y/PPM);
    wheel = world->CreateBody( &bodyDef );
    wheel->CreateFixture(&fixtureDef);
    
    b2RevoluteJointDef revoluteJointDef;
    revoluteJointDef.bodyA = body;
    revoluteJointDef.bodyB = wheel;
    revoluteJointDef.collideConnected = false;
    revoluteJointDef.localAnchorA.Set(0,-player.size.y/2.0f/PPM);//the top right corner of the box
    revoluteJointDef.localAnchorB.Set(0,0);//center of the circle
    revoluteJointDef.enableMotor = true;
    revoluteJointDef.maxMotorTorque = 10;
    revoluteJointDef.motorSpeed = 0;
    joint = (b2RevoluteJoint*)world->CreateJoint( &revoluteJointDef );
  
    // Platform
    {
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        
        bd.position.Set(this->Width/2.0/PPM, 400.0f/PPM);
        b2Body* body = world->CreateBody(&bd);
        
        bd.position.Set((this->Width/2.0+100)/PPM, 300.0f/PPM);
        b2Body* body2 = world->CreateBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox(300.0f/PPM, 2.0f/PPM);
        body->CreateFixture(&shape, 1.0f);
        
        shape.SetAsBox(20.0f/PPM, 40.0f/PPM);
        body2->CreateFixture(&shape, 1.0f);
    }
 
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(this->Width/2.0/PPM, 100.0f/PPM);
 
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(this->Width/2.0f/PPM, 100.0f/PPM);
    groundBody->CreateFixture(&groundBox, 0.0f);
    
}

void GameB2Platformer::OnEnter(){
    glm::mat4 spriteProjection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                            0.0f, static_cast<GLfloat>(this->Height),
                                            -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("projection", spriteProjection);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("projection", spriteProjection);
}

void GameB2Platformer::OnExit(){
    
}

void GameB2Platformer::KeyboardInput(ExKeyCode keycode, ExKeyAction action)
{
    
}

void GameB2Platformer::Update(GLfloat dt)
{
    if(GoButton_Right->mouseState == MOUSE_PRESSED)
    {
//        if(body->GetLinearVelocity().x <= 2)
//            body->ApplyLinearImpulse({0.1f,0}, body->GetWorldCenter(), true);
            
        joint->SetMotorSpeed(-360 * DEG2RAD);
    }
    else if(GoButton_Left->mouseState == MOUSE_PRESSED)
    {
//        if (body->GetLinearVelocity().x >= -2)
//            body->ApplyLinearImpulse({-0.1f,0}, body->GetWorldCenter(), true);
        
        joint->SetMotorSpeed(360 * DEG2RAD);
    }
    else{
        joint->SetMotorSpeed(0);
    }
    
    if(JumpButton->mouseState == MOUSE_PRESSED)
    {
        if (player.canJump)
        {
            player.canJump = false;
            body->ApplyLinearImpulse({0,1.2f}, body->GetWorldCenter(), true);
        }
    }
    
    if(body->GetLinearVelocity().y==0){
        player.canJump = true;
    }
    
    float timeStep = 1.0f / 60.0f;
    world->Step(timeStep, 6, 2);
    
    b2Vec2 pos = body->GetPosition();
    player.position = {pos.x * PPM - player.size.x/2.0f , pos.y * PPM - player.size.y/2.0f};
    
    //UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
    
    //UpdateCameraCenter_Demo(&camera, &player, this->Width, this->Height);
    UpdateCameraCenterInsideMap(&camera, &player,
                                envItems_b2, envItemsLength_b2,
                                this->Width, this->Height);
    //UpdateCameraCenterSmoothFollow_Demo(&camera, &player, deltaTime, this->Width, this->Height);
}

void GameB2Platformer::Render()
{
    Camera2D camera_b2;
    camera_b2.target = camera.target/PPM;
    camera_b2.offset = camera.offset/PPM;
    camera_b2.rotation = camera.rotation;
    camera_b2.zoom = camera.zoom;
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", GetCameraMatrix2D(camera));
    
    for (int i = 0; i < envItemsLength_b2; i++)
    {
        EnvItem item = envItems_b2[i];
        glm::vec2 pos = {item.position.x+item.size.x/2.0f,item.position.y+item.size.y/2.0f};
        colorRenderer->DrawColor(envItems_b2[i].color, pos,envItems_b2[i].size);
    }

    colorRenderer->DrawColor( ORANGE,player.position, player.size);
    
    GoButton_Left->Draw(*uiRenderer);
    GoButton_Right->Draw(*uiRenderer);
    JumpButton->Draw(*uiRenderer);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    g_debugDraw.SetFlags(flags);
    world->DebugDraw();
    g_debugDraw.Flush(GetCameraMatrix2D(camera_b2));
}

void GameB2Platformer::Release()
{
    
}

void UpdateCameraCenter(Camera2D *camera, GameB2Platformer::Player *player, int width, int height)
{
    camera->offset = { width/2.0f, height/2.0f };
    camera->target = player->position;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, GameB2Platformer::Player *player, GameB2Platformer::EnvItem *envItems, int envItemsLength, int width, int height)
{
    camera->target = player->position;
    camera->offset = { width/2.0f, height/2.0f };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;
    for (int i = 0; i < envItemsLength; i++)
    {
        GameB2Platformer::EnvItem *ei = envItems + i;
        minX = fminf(ei->position.x, minX);
        maxX = fmaxf(ei->position.x + ei->size.x, maxX);
        minY = fminf(ei->position.y, minY);
        maxY = fmaxf(ei->position.y + ei->size.y, maxY);
    }

    glm::vec2 max = GetWorldToScreen2D({ maxX, maxY }, *camera);
    glm::vec2 min = GetWorldToScreen2D({ minX, minY }, *camera);

    if (max.x < width) camera->offset.x = width - (max.x - width/2);
    if (max.y < height) camera->offset.y = height - (max.y - height/2);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
}

void UpdateCameraCenterSmoothFollow(Camera2D *camera, GameB2Platformer::Player *player, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = { width/2.0f, height/2.0f };
    glm::vec2 diff = player->position - camera->target;
    float length = glm::length(diff);

    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = camera->target+diff*(speed*delta/length);
    }
}
