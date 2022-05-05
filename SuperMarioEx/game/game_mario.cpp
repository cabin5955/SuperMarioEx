//
//  game_platformer.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/3.
//

#include "game_mario.hpp"

#define PLAYER_HOR_SPD 200.0f
#define ZOOM 2.0f

GameMario* GameMario::s_instance = nullptr;

void GoLeft_mario()
{
    
}

void GoRight_mario()
{
    
}

void Jump_mario()
{
    
}

void UpdateCameraCenter(Camera2D *camera, Player *player, int width, int height);

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, int width, int height);

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, float delta, int width, int height);

void GameMario::Init(unsigned int width, unsigned int height)
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
                                GoRight_mario,0);
    
    GoButton_Left = new Button(this,glm::vec2(this->Width/2-btn_size*scal-btn_size*4,this->Height-128*scal),
                               glm::vec2(btn_size*scal,btn_size*scal),
                               180.0f*DEG2RAD,
                               ResourceManager::GetTexture("arrow"),
                               GoLeft_mario,0);
    
    JumpButton = new Button(this,glm::vec2(this->Width-btn_size*4,this->Height-128*scal),
                            glm::vec2(btn_size*scal,btn_size*scal),
                            -90.0f*DEG2RAD,
                            ResourceManager::GetTexture("arrow"),
                            Jump_mario,0);
    
    player = new Player({ 260, 100 });
    player->speed = 0;
    player->canJump = false;
    
    camera.target = {player->Position.x*ZOOM,player->Position.y*ZOOM};
    camera.rotation = 0.0f;
    camera.offset = { width/(2.0f*ZOOM), height/(2.0f*ZOOM)};
    camera.zoom = ZOOM;
}

void GameMario::OnEnter(){
    
    char des[1024] = {0};
    tmx_img_load_func = TilemapHelper::ex_tex_loader;
    tmx_img_free_func = TilemapHelper::ex_free_tex;
    tmx_map *map = tmx_load(Global::ResFullPath(des,"level1.tmx"));
    if (!map) {
        tmx_perror("Cannot load map");
    }else{
        TilemapHelper::get_tilemap_all_layers(map, map->ly_head);
    }
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("projection", projection);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("projection", projection);
    
    Shader uiShader = ResourceManager::GetShader("ui");
    uiShader.use();
    uiShader.setMat4("projection", projection);
}

void GameMario::OnExit(){
    
}

void GameMario::KeyboardInput(int virtual_key, char pressed)
{
    
}

void GameMario::Update(GLfloat dt)
{
    float deltaTime = Global::GetFrameTime();
    if(GoButton_Right->mouseState == MOUSE_PRESSED){
        player->Position.x += PLAYER_HOR_SPD*deltaTime;
        player->isWalk = true;
        player->flipX = false;
    }
    else if(GoButton_Left->mouseState == MOUSE_PRESSED){
        float step = PLAYER_HOR_SPD*deltaTime;
        player->Position.x -= step;
        player->isWalk = true;
        player->flipX = true;
    }
    else{
        player->isWalk = false;
    }
    
    if(JumpButton->mouseState == MOUSE_PRESSED){
        if (player->canJump){
            player->speed = -PLAYER_JUMP_SPD;
            player->canJump = false;
            player->isJump = true;
        }
        else{
            player->isWalk = false;
        }
    }
    
    player->Update(&TilemapHelper::gameItems[0], (int)TilemapHelper::gameItems.size());
    
    //UpdateCameraCenter(&camera, &player, this->Width, this->Height);
    UpdateCameraCenterInsideMap(&camera, player,
                                &TilemapHelper::gameItems[0], (int)TilemapHelper::gameItems.size(),
                                this->Width, this->Height);
    //UpdateCameraCenterSmoothFollow(&camera, &player, deltaTime, this->Width, this->Height);
    
}

void GameMario::Render()
{
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", GetCameraMatrix2D(camera));
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("view", GetCameraMatrix2D(camera));
    
    Shader tilemapShader = ResourceManager::GetShader("tilebatch");
    tilemapShader.use();
    tilemapShader.setMat4("view", GetCameraMatrix2D(camera));
    for(int i = 0; i < TilemapHelper::mapLayers.size(); i++)
    {
        TilemapHelper::mapLayers[i]->draw({0.0f,0.0f});
    }
    
    for (int i = 0; i < TilemapHelper::gameItems.size(); i++)
    {
        glm::vec2 pos = {TilemapHelper::gameItems[i].position.x,TilemapHelper::gameItems[i].position.y};
        colorRenderer->DrawColor(TilemapHelper::gameItems[i].color, pos ,TilemapHelper::gameItems[i].size);
    }
    
    player->Draw(*spriteRenderer);
    
//    for(int i = 0; i < tiles.size(); i++)
//    {
//        Renderer->DrawTile(*tiles[i]);
//    }
    
    GoButton_Left->Draw(*uiRenderer);
    GoButton_Right->Draw(*uiRenderer);
    JumpButton->Draw(*uiRenderer);
}

void GameMario::Release()
{
    
}

void UpdateCameraCenter(Camera2D *camera, Player *player, int width, int height)
{
    camera->offset = { width/2.0f, height/2.0f };
    camera->target = player->Position;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, int width, int height)
{
    camera->target = {player->Position.x*ZOOM,player->Position.y*ZOOM};
    camera->offset = { width/(2.0f*ZOOM), height/(2.0f*ZOOM) };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->position.x, minX);
        maxX = fmaxf(ei->position.x + ei->size.x, maxX);
        minY = fminf(ei->position.y, minY);
        maxY = fmaxf(ei->position.y + ei->size.y, maxY);
    }

    glm::vec2 max = GetWorldToScreen2D({ maxX, maxY }, *camera);
    glm::vec2 min = GetWorldToScreen2D({ minX, minY }, *camera);
    if (max.x < width) {
        camera->offset.x = (width - (max.x - width/2))/ZOOM;
    }
    if (max.y < height){
        camera->offset.y = (height - (max.y - height/2))/ZOOM;
    }
    if (min.x > 0) {
        camera->offset.x = (width/2 - min.x)/ZOOM;
    }
    if (min.y > 0) {
        camera->offset.y = (height/2 - min.y)/ZOOM;
    }
}

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = { width/2.0f, height/2.0f };
    glm::vec2 pos = {player->Position.x, player->Position.y};
    glm::vec2 diff = pos - camera->target;
    float length = glm::length(diff);

    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = camera->target+diff*(speed*delta/length);
    }
}

