//
//  game_b2unicycle_mario.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/10.
//

#include "game_b2unicycle_mario.hpp"
#include "world_contact_listener.hpp"
#include "map2body_builder.hpp"
#include "debugdrawex.hpp"

#define PLAYER_HOR_SPD 200.0f

bool isb2DebugDraw_unicycle = false;
GameB2UnicycleMario* GameB2UnicycleMario::s_instance = nullptr;
int zoomStart= 0;
int rotationStart = 0;

WorldContactListener unicycleMarioContactListener;

void GoLeft_b2unicycle_mario()
{
    GameB2UnicycleMario::GetInstance()->PlayerMoveBack();
}

void GoRight_b2unicycle_mario()
{
    GameB2UnicycleMario::GetInstance()->PlayerMoveForward();
}

void Jump_b2unicycle_mario()
{
    GameB2UnicycleMario::GetInstance()->PlayerJump();
}

void StopMove_b2unicycle_mario()
{
    GameB2UnicycleMario::GetInstance()->PlayerStopMove();
}

void GameB2UnicycleMario::Init(unsigned int width, unsigned int height)
{
    this->Width = width;
    this->Height = height;
    char des[1024] = {0};
    
    ResourceManager::LoadTexture(Global::ResFullPath(des, "oposum.png"), GL_TRUE, "oposum");
    ResourceManager::LoadTexture(Global::ResFullPath(des, "goomba.png"), GL_TRUE, "goomba");
    ResourceManager::LoadTexture(Global::ResFullPath(des, "turtle.png"), GL_TRUE, "turtle");
    
    // 配置着色器
    glm::mat4 projection2d = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("view", glm::mat4(1.0));
    spriteShader.setMat4("projection", projection2d);
    
    Shader textShader = ResourceManager::GetShader("text");
    Text = new TextRenderer(textShader, this->Width, this->Height);
    Text->Load(Global::ResFullPath(des,"default.ttf"),48);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", glm::mat4(1.0));
    colorShader.setMat4("projection", projection2d);
    
    Shader uiShader = ResourceManager::GetShader("ui");
    uiShader.use();
    uiShader.setMat4("projection", projection2d);
    
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
                                GoRight_b2unicycle_mario,StopMove_b2unicycle_mario);
    
    GoButton_Left = new Button(this,glm::vec2(this->Width/2-btn_size*scal-btn_size*4,this->Height-128*scal),
                               glm::vec2(btn_size*scal,btn_size*scal),
                               180.0f*DEG2RAD,
                               ResourceManager::GetTexture("arrow"),
                               GoLeft_b2unicycle_mario,StopMove_b2unicycle_mario);
    
    JumpButton = new Button(this,glm::vec2(this->Width-btn_size*4,this->Height-128*scal),
                            glm::vec2(btn_size*scal,btn_size*scal),
                            -90.0f*DEG2RAD,
                            ResourceManager::GetTexture("arrow"),
                            Jump_b2unicycle_mario,0);
    
    b2Vec2 gravity = b2Vec2(0.0f, -8.0f);
    world = new b2World(gravity);
    if(isb2DebugDraw_unicycle)
    {
        g_debugDraw.Create();
        world->SetDebugDraw(&g_debugDraw);
    }
    else{
        g_DebugDrawEx.Create();
    }
    world->SetContactListener(&unicycleMarioContactListener);
    
    player = new B2UnicyclePlayer(world, { 270, 60 });
    player->speed = 0;
    player->canJump = false;
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(this->Width/2.0/PPM, 0.0f/PPM);
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(this->Width/2.0f/PPM, 5.0f/PPM);
    groundBody->CreateFixture(&groundBox, 0.0f);
    
    // Platform
    {
        b2BodyDef bd;
        bd.position.Set(2000.0f/PPM, 300.0f/PPM);
        b2Body* body = world->CreateBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox(4000.0f/PPM, 10.0f/PPM);
        b2FixtureDef fdef;
        fdef.filter.categoryBits = Player::PLATFORM_BIT;
        fdef.shape = &shape;
        body->CreateFixture(&fdef);
    }
    
    ZOOM = 2.0f;
    camera.target = {player->Position.x*ZOOM,player->Position.y*ZOOM};
    camera.rotation = 0.0f;
    camera.offset = { width/(2.0f*ZOOM), height/(2.0f*ZOOM)};
    camera.zoom = ZOOM;
}

void GameB2UnicycleMario::OnEnter(){
    
    char des[1024] = {0};
    tmx_img_load_func = TilemapHelper::ex_tex_loader;
    tmx_img_free_func = TilemapHelper::ex_free_tex;
    tmx_map *map = tmx_load(Global::ResFullPath(des,"mysunnyland.tmx"));//"level1.tmx"));//"mysunnyland.tmx"));
    goombaNum = 0;
    turtleNum = 0;
    if (!map) {
        tmx_perror("Cannot load map");
    }else{
        TilemapHelper::get_tilemap_all_layers(map, map->ly_head);
    }
    
    for (int i = 0; i < TilemapHelper::gameItems.size(); i++)
    {
        EnvItem *item = &TilemapHelper::gameItems[i];
        if(strcmp(item->name,"Bricks")==0){
            this->brickList.push_back(new Brick(this->world, map, item));
        }
        else if(strcmp(item->name,"Coins")==0){
            this->coinList.push_back(new Coin(this->world, map, item));
        }
        else if(strcmp(item->name,"Goombas")==0){
            glm::vec2 pos = {item->position.x,item->position.y};
            goombas[goombaNum] = new Goomba(world, pos);
            goombaNum++;
        }
        else if(strcmp(item->name,"Turtles")==0){
            glm::vec2 pos = {item->position.x,item->position.y};
            turtles[turtleNum] = new Turtle(world, pos);
            turtleNum++;
        }
        else{
            Map2BodyBuilder::BuildEnvItemObject(map, this->world, item);
        }
    }
    
    Map2BodyBuilder::BuildAllPolylineObjects(this->world);
    Map2BodyBuilder::BuildAllPolygonObjects(this->world);
    
    glm::mat4 projection2d = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("projection", projection2d);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("projection", projection2d);
    
    Shader uiShader = ResourceManager::GetShader("ui");
    uiShader.use();
    uiShader.setMat4("projection", projection2d);
}

void GameB2UnicycleMario::OnExit(){
    
}

void GameB2UnicycleMario::PlayerMoveForward(){
    player->flipX = false;
    player->isWalk = true;
    player->joint->SetMotorSpeed(-540 * DEG2RAD);
}

void GameB2UnicycleMario::PlayerMoveBack(){
    player->flipX = true;
    player->isWalk = true;
    player->joint->SetMotorSpeed(540 * DEG2RAD);
}

void GameB2UnicycleMario::PlayerStopMove(){
    player->isWalk = false;
    player->joint->SetMotorSpeed(0);
    rotationStart = 1;
}

void GameB2UnicycleMario::PlayerJump(){
    rotationStart = 2;
    return;
    b2Body *body = player->body;
    if (player->canJump){
        player->canJump = false;
        player->isJump = true;
        body->ApplyLinearImpulse({0,0.07f}, body->GetWorldCenter(), true);
    }
}

void GameB2UnicycleMario::KeyboardInput(ExKeyCode keyCode, ExKeyAction action)
{
    if(action == ExActionPressed)
    {
        switch (keyCode) {
            case ExKeyLeft:
                PlayerMoveBack();
                break;
            case ExKeyRight:
                PlayerMoveForward();
                break;
            case ExKeyUp:
                PlayerJump();
                break;
            case ExKeyDown:
                PlayerStopMove();
                break;
            default:
                break;
        }
    }
}

void GameB2UnicycleMario::Update(GLfloat dt)
{
    b2Body *body = player->body;
    if(GoButton_Right->mouseState == MOUSE_PRESSED)
    {
        PlayerMoveForward();
        if(player->isJump && body->GetLinearVelocity().x <= 1){
            body->ApplyLinearImpulse({0.01f,0}, body->GetWorldCenter(), true);
        }
    }
    else if(GoButton_Left->mouseState == MOUSE_PRESSED)
    {
        PlayerMoveBack();
        if (player->isJump &&body->GetLinearVelocity().x >= -1){
            body->ApplyLinearImpulse({-0.01f,0}, body->GetWorldCenter(), true);
        }
    }
    
    //player->Update(&TilemapHelper::gameItems[0], (int)TilemapHelper::gameItems.size());
    player->Update();
    
    for (int i=0; i<goombaNum; i++) {
        goombas[i]->Update(dt);
    }
    
    for (int i=0; i<turtleNum; i++)
    {
        turtles[i]->Update(dt);
    }
    
    //UpdateCameraCenter(&camera, &player, this->Width, this->Height);
    UpdateCameraCenterInsideMap();
    //UpdateCameraCenterSmoothFollow(&camera, &player, deltaTime, this->Width, this->Height);
    
    float timeStep = 1.0f / 60.0f;
    world->Step(timeStep, 6, 2);
    
    b2Vec2 pos = body->GetPosition();
    player->Position = {pos.x*PPM-0.5f*player->Size.x,this->Height-pos.y*PPM+0.4f*player->Size.y, 0};
    
    //if(zoomStart==1 && ZOOM > 1.0f){ ZOOM -= dt*0.5f;camera.zoom = ZOOM;}
    if(rotationStart==1 && camera.rotation < 20.0f){ camera.rotation += dt*10;}
    if(rotationStart==2 && camera.rotation > -20.0f){ camera.rotation -= dt*10;}
}

void GameB2UnicycleMario::Render()
{
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", GetCameraMatrix2D(camera));
    //colorShader.setMat4("view",glm::mat4(1.0f));
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("view", GetCameraMatrix2D(camera));
    //spriteShader.setMat4("view",glm::mat4(1.0f));
    
    Shader tilemapShader = ResourceManager::GetShader("tilebatch");
    tilemapShader.use();
    tilemapShader.setMat4("view", GetCameraMatrix2D(camera));
    //tilemapShader.setMat4("view",glm::mat4(1.0f));
    for(int i = 0; i < TilemapHelper::mapLayers.size(); i++)
    {
        TilemapHelper::mapLayers[i]->draw({0.0f,0.0f});
    }
    
    glm::vec2 pos = {player->Position.x,player->Position.y-player->Size.y+4.0f};
    player->Draw(*spriteRenderer,pos);
    
    //spriteRenderer->DrawSprite(*oposumSub, {100,100},{36,28});
    for (int i=0; i<goombaNum; i++)
    {
        goombas[i]->Draw(*spriteRenderer);
    }
    
    for (int i=0; i<turtleNum; i++)
    {
        turtles[i]->Draw(*spriteRenderer);
    }
    
    GoButton_Left->Draw(*uiRenderer);
    GoButton_Right->Draw(*uiRenderer);
    JumpButton->Draw(*uiRenderer);
    
    Camera2D camera_b2;
    float my = (ZOOM-1.0f) * this->Height;
    camera_b2.target = {camera.target.x/PPM, (this->Height-camera.target.y)/PPM};
    camera_b2.offset = {camera.offset.x/PPM,(this->Height-camera.offset.y-my)/PPM};
    camera_b2.rotation = camera.rotation;
    camera_b2.zoom = camera.zoom;
    
    if(isb2DebugDraw_unicycle)
    {
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        flags += b2Draw::e_jointBit;
        g_debugDraw.SetFlags(flags);
        world->DebugDraw();
        g_debugDraw.Flush(GetCameraMatrix2D(camera_b2));
    }
    else{
        g_DebugDrawEx.DrawCircle({0,0}, 0.1f, exColor(1.0f,0,0));
        g_DebugDrawEx.Flush(GetCameraMatrix2D(camera_b2));
    }
    
    int d_fps = (int)this->fps;
    std::stringstream ss; ss << d_fps;
    Text->RenderText("fps:"+ss.str(), 200.0f, 10.0f, 0.75f, glm::vec3(1.0f,1.0f,1.0f));
}

void GameB2UnicycleMario::Release()
{
    
}

void GameB2UnicycleMario::UpdateCameraCenterInsideMap()
{
    float width = this->Width;
    float height = this->Height;
    camera.target = {player->Position.x*ZOOM,player->Position.y*ZOOM};
    camera.offset = { width/(2.0f*ZOOM), height/(2.0f*ZOOM) };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;
    for (int i = 0; i < TilemapHelper::gameItems.size(); i++)
    {
        EnvItem *ei = &TilemapHelper::gameItems[i];
        minX = fminf(ei->position.x, minX);
        maxX = fmaxf(ei->position.x + ei->size.x, maxX);
        minY = fminf(ei->position.y, minY);
        maxY = fmaxf(ei->position.y + ei->size.y, maxY);
    }

    glm::vec2 max = GetWorldToScreen2D({ maxX, maxY }, camera);
    glm::vec2 min = GetWorldToScreen2D({ minX, minY }, camera);
    if (max.x < width) {
        camera.offset.x = (width - (max.x - width/2))/ZOOM;
    }
    if (max.y < height){
        //camera.offset.y = (height - (max.y - height/2))/ZOOM;
    }
    if (min.x > 0) {
        camera.offset.x = (width/2 - min.x)/ZOOM;
    }
    if (min.y > 0) {
        //camera.offset.y = (height/2 - min.y)/ZOOM;
    }
}
