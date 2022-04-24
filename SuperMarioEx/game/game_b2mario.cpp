//
//  game_b2mario.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/13.
//

#include "game_b2mario.hpp"

#define PLAYER_HOR_SPD 200.0f
float ZOOM = 2.0f;

GameB2Mario* GameB2Mario::s_instance = nullptr;

void GoLeft_b2mario()
{
    
}

void GoRight_b2mario()
{
    
}

void Jump_b2mario()
{
    
}

void GameB2Mario::Init(unsigned int width, unsigned int height)
{
    this->Width = width;
    this->Height = height;
    char des[1024] = {0};
    
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
                                GoRight_b2mario,0);
    
    GoButton_Left = new Button(this,glm::vec2(this->Width/2-btn_size*scal-btn_size*4,this->Height-128*scal),
                               glm::vec2(btn_size*scal,btn_size*scal),
                               180.0f*DEG2RAD,
                               ResourceManager::GetTexture("arrow"),
                               GoLeft_b2mario,0);
    
    JumpButton = new Button(this,glm::vec2(this->Width-btn_size*4,this->Height-128*scal),
                            glm::vec2(btn_size*scal,btn_size*scal),
                            -90.0f*DEG2RAD,
                            ResourceManager::GetTexture("arrow"),
                            Jump_b2mario,0);
    
    player = new Player({ 300, 100 });
    player->speed = 0;
    player->canJump = false;
    
    camera.target = {player->Position.x*ZOOM,player->Position.y*ZOOM};
    camera.rotation = 0.0f;
    camera.offset = { width/(2.0f*ZOOM), height/(2.0f*ZOOM)};
    camera.zoom = ZOOM;
    
    g_debugDraw.Create();
    
    b2Vec2 gravity = b2Vec2(0.0f, -8.0f);
    world = new b2World(gravity);
    world->SetDebugDraw(&g_debugDraw);
    
    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(player->Position.x/PPM,(this->Height-player->Position.y)/PPM);
    playerBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&player);;
    body = world->CreateBody(&playerBodyDef);
    
    b2PolygonShape playerShape;
    playerShape.SetAsBox(20/2.0f/PPM, 20/2.0f/PPM);
    body->CreateFixture(&playerShape, 1.0f);
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(this->Width/2.0/PPM, 0.0f/PPM);
 
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(this->Width/2.0f/PPM, 5.0f/PPM);
    groundBody->CreateFixture(&groundBox, 0.0f);
}

void GameB2Mario::OnEnter(){
    
    char des[1024] = {0};
    tmx_img_load_func = TilemapHelper::ex_tex_loader;
    tmx_img_free_func = TilemapHelper::ex_free_tex;
    tmx_map *map = tmx_load(Global::ResFullPath(des,"level1.tmx"));
    if (!map) {
        tmx_perror("Cannot load map");
    }else{
        TilemapHelper::get_tilemap_all_layers(map, map->ly_head);
    }
    
    for (int i = 0; i < TilemapHelper::gameItems.size(); i++)
    {
        glm::vec2 pos = {TilemapHelper::gameItems[i].position.x,TilemapHelper::gameItems[i].position.y};
        glm::vec2 size = {TilemapHelper::gameItems[i].size.x,TilemapHelper::gameItems[i].size.y};
        b2BodyDef bd;        
        int map_y = map->height-(pos.y+size.y)/map->tile_height;
        int py = map_y * map->tile_height + size.y/2.0f;
        float base = this->Height - (map->height)*map->tile_height;
        bd.position.Set((pos.x+size.x/2.0f)/PPM, (py+base)/PPM);
        b2Body* body = world->CreateBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox(size.x/2.0f/PPM, size.y/2.0f/PPM);
        body->CreateFixture(&shape, 0.0f);
    }
    
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

void GameB2Mario::OnExit(){
    
}

void GameB2Mario::KeyboardInput(int virtual_key, char pressed)
{
    
}

void GameB2Mario::Update(GLfloat dt)
{
    if(GoButton_Right->mouseState == MOUSE_PRESSED)
    {
        player->flipX = false;
        if(body->GetLinearVelocity().x <= 1)
            body->ApplyLinearImpulse({0.1f,0}, body->GetWorldCenter(), true);
    }
    else if(GoButton_Left->mouseState == MOUSE_PRESSED)
    {
        player->flipX = true;
        if (body->GetLinearVelocity().x >= -1)
            body->ApplyLinearImpulse({-0.1f,0}, body->GetWorldCenter(), true);
    }
    if(JumpButton->mouseState == MOUSE_PRESSED)
    {
        if (player->canJump){
            body->ApplyLinearImpulse({0,0.16f}, body->GetWorldCenter(), true);
        }
    }
    
    if(abs(body->GetLinearVelocity().y) < 0.001f){
        player->canJump = true;
        player->isJump = false;
    }
    else{
        player->canJump = false;
        player->isJump = true;
    }
    
    if (abs(body->GetLinearVelocity().x) < 0.001f){
        player->isWalk = false;
    }
    else{
        player->isWalk = true;
    }
    
    //player->Update(&TilemapHelper::gameItems[0], (int)TilemapHelper::gameItems.size());
    player->Update();
    
    //UpdateCameraCenter(&camera, &player, this->Width, this->Height);
    UpdateCameraCenterInsideMap();
    //UpdateCameraCenterSmoothFollow(&camera, &player, deltaTime, this->Width, this->Height);
    
    float timeStep = 1.0f / 60.0f;
    world->Step(timeStep, 6, 2);
    
    b2Vec2 pos = body->GetPosition();
    player->Position = {pos.x*PPM-0.5f*player->Size.x,this->Height-pos.y*PPM+0.4f*player->Size.y, 0};
    
    //if(ZOOM < 2.0f){ ZOOM += 0.002f;camera.zoom = ZOOM;}
}

void GameB2Mario::Render()
{
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", GetCameraMatrix2D(camera));
    //colorShader.setMat4("view",glm::mat4(1.0f));
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("view", GetCameraMatrix2D(camera));
    //spriteShader.setMat4("view",glm::mat4(1.0f));
    
    Shader tilemapShader = ResourceManager::GetShader("tilemap");
    tilemapShader.use();
    tilemapShader.setMat4("view", GetCameraMatrix2D(camera));
    //tilemapShader.setMat4("view",glm::mat4(1.0f));
    for(int i = 0; i < TilemapHelper::mapLayers.size(); i++)
    {
        TilemapHelper::mapLayers[i]->draw({0.0f,0.0f});
    }
    
//    for (int i = 0; i < TilemapHelper::gameItems.size(); i++)
//    {
//        glm::vec2 pos = {TilemapHelper::gameItems[i].position.x,TilemapHelper::gameItems[i].position.y};
//        colorRenderer->DrawColor(TilemapHelper::gameItems[i].color, pos ,TilemapHelper::gameItems[i].size);
//    }
    
    player->Draw(*spriteRenderer);
    
//    for(int i = 0; i < tiles.size(); i++)
//    {
//        Renderer->DrawTile(*tiles[i]);
//    }
    
    GoButton_Left->Draw(*uiRenderer);
    GoButton_Right->Draw(*uiRenderer);
    JumpButton->Draw(*uiRenderer);
    
    Camera2D camera_b2;
    float my = (ZOOM-1.0f) * this->Height;
    camera_b2.target = {camera.target.x/PPM, (this->Height-camera.target.y)/PPM};
    camera_b2.offset = {camera.offset.x/PPM,(this->Height-camera.offset.y-my)/PPM};
    camera_b2.rotation = camera.rotation;
    camera_b2.zoom = camera.zoom;
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    g_debugDraw.SetFlags(flags);
    world->DebugDraw();
    g_debugDraw.Flush(GetCameraMatrix2D(camera_b2));
    //g_debugDraw.Flush(glm::mat4(1.0f));
}

void GameB2Mario::Release()
{
    
}

void GameB2Mario::UpdateCameraCenterInsideMap()
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
        camera.offset.y = (height - (max.y - height/2))/ZOOM;
    }
    if (min.x > 0) {
        camera.offset.x = (width/2 - min.x)/ZOOM;
    }
    if (min.y > 0) {
        camera.offset.y = (height/2 - min.y)/ZOOM;
    }
}

