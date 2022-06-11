//
//  game_b2mario.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/13.
//

#include "game_b2mario.hpp"
#include "world_contact_listener.hpp"
#include "map2body_builder.hpp"

#define PLAYER_HOR_SPD 200.0f
float ZOOM = 2.0f;

bool isb2DebugDraw = true;
GameB2Mario* GameB2Mario::s_instance = nullptr;

WorldContactListener myContactListenerInstance;

void GoLeft_b2mario()
{
    GameB2Mario::GetInstance()->PlayerMoveBack();
}

void GoRight_b2mario()
{
    GameB2Mario::GetInstance()->PlayerMoveForward();
}

void Jump_b2mario()
{
    GameB2Mario::GetInstance()->PlayerJump();
}

void StopMove_b2mario()
{
    GameB2Mario::GetInstance()->PlayerStopMove();
}

void GameB2Mario::Init(unsigned int width, unsigned int height)
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
                                GoRight_b2mario,StopMove_b2mario);
    
    GoButton_Left = new Button(this,glm::vec2(this->Width/2-btn_size*scal-btn_size*4,this->Height-128*scal),
                               glm::vec2(btn_size*scal,btn_size*scal),
                               180.0f*DEG2RAD,
                               ResourceManager::GetTexture("arrow"),
                               GoLeft_b2mario,StopMove_b2mario);
    
    JumpButton = new Button(this,glm::vec2(this->Width-btn_size*4,this->Height-128*scal),
                            glm::vec2(btn_size*scal,btn_size*scal),
                            -90.0f*DEG2RAD,
                            ResourceManager::GetTexture("arrow"),
                            Jump_b2mario,0);
    
    b2Vec2 gravity = b2Vec2(0.0f, -8.0f);
    world = new b2World(gravity);
    if(isb2DebugDraw)
    {
        g_debugDraw.Create();
        world->SetDebugDraw(&g_debugDraw);
    }
    world->SetContactListener(&myContactListenerInstance);
    
    player = new B2Player(world, { 270, 60 });
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
    
    camera.target = {player->Position.x*ZOOM,player->Position.y*ZOOM};
    camera.rotation = 0.0f;
    camera.offset = { width/(2.0f*ZOOM), height/(2.0f*ZOOM)};
    camera.zoom = ZOOM;

}

void GameB2Mario::OnEnter(){
    
    char des[1024] = {0};
    tmx_img_load_func = TilemapHelper::ex_tex_loader;
    tmx_img_free_func = TilemapHelper::ex_free_tex;
    tmx_map *map = tmx_load(Global::ResFullPath(des,"mario_level1.tmx"));//"mysunnyland.tmx"));
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

void GameB2Mario::OnExit(){
    
}

void GameB2Mario::PlayerMoveForward(){
    b2Body *body = player->body;
    player->flipX = false;
    player->isWalk = true;
    if(body->GetLinearVelocity().x <= 1){
        body->ApplyLinearImpulse({0.01f,0}, body->GetWorldCenter(), true);
    }
}

void GameB2Mario::PlayerMoveBack(){
    b2Body *body = player->body;
    player->flipX = true;
    player->isWalk = true;
    if (body->GetLinearVelocity().x >= -1){
        body->ApplyLinearImpulse({-0.01f,0}, body->GetWorldCenter(), true);
    }
}

void GameB2Mario::PlayerStopMove(){
    
}

void GameB2Mario::PlayerJump(){
    b2Body *body = player->body;
    if (player->canJump){
        body->ApplyLinearImpulse({0,0.07f}, body->GetWorldCenter(), true);
    }
}

void GameB2Mario::KeyboardInput(ExKeyCode keyCode, ExKeyAction action)
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

void GameB2Mario::Update(GLfloat dt)
{
    b2Body *body = player->body;
    if(GoButton_Right->mouseState == MOUSE_PRESSED)
    {
        PlayerMoveForward();
    }
    else if(GoButton_Left->mouseState == MOUSE_PRESSED)
    {
        PlayerMoveBack();
    }
    
    if (abs(body->GetLinearVelocity().x) < 0.05f){
        player->isWalk = false;
    }
    else{
        player->isWalk = true;
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
    
    Shader tilemapShader = ResourceManager::GetShader("tilebatch");
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
    
    //spriteRenderer->DrawSprite(*oposumSub, {100,100},{36,28});
    for (int i=0; i<goombaNum; i++)
    {
        goombas[i]->Draw(*spriteRenderer);
    }
    
    for (int i=0; i<turtleNum; i++)
    {
        turtles[i]->Draw(*spriteRenderer);
    }
    
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
    
    if(isb2DebugDraw)
    {
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        flags += b2Draw::e_jointBit;
        g_debugDraw.SetFlags(flags);
        world->DebugDraw();
        g_debugDraw.Flush(GetCameraMatrix2D(camera_b2));
        
        //g_debugDraw.Flush(glm::mat4(1.0f));
    }
    int d_fps = (int)this->fps;
    std::stringstream ss; ss << d_fps;
    Text->RenderText("fps:"+ss.str(), 200.0f, 10.0f, 0.75f, glm::vec3(1.0f,1.0f,1.0f));
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
        //camera.offset.y = (height - (max.y - height/2))/ZOOM;
    }
    if (min.x > 0) {
        camera.offset.x = (width/2 - min.x)/ZOOM;
    }
    if (min.y > 0) {
        //camera.offset.y = (height/2 - min.y)/ZOOM;
    }
}

