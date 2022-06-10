//
//  game_tilemap.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/27.
//

#include "game_tilemap.hpp"
#define ZOOM 2.0f

GameTilemap* GameTilemap::s_instance = nullptr;

void GameTilemap::Init(unsigned int width, unsigned int height)
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
    
    glm::vec2 pos = { 420, 200 };
    camera.target = {pos.x * ZOOM,pos.y * ZOOM};
    camera.rotation = 0.0f;
    camera.offset = { width/(2.0f*ZOOM), height/(2.0f*ZOOM)};
    camera.zoom = ZOOM;
}

void GameTilemap::OnEnter(){
    
    char des[1024] = {0};
    tmx_img_load_func = TilemapHelper::ex_tex_loader;
    tmx_img_free_func = TilemapHelper::ex_free_tex;
    tmx_map *map = tmx_load(Global::ResFullPath(des,"sunnyland_level1.tmx"));
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

void GameTilemap::OnExit(){
    
}

void GameTilemap::KeyboardInput(ExKeyCode keycode, ExKeyAction action)
{
    
}

void GameTilemap::Update(GLfloat dt)
{
    
}

void GameTilemap::Render()
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
    
//    for(int i = 0; i < tiles.size(); i++)
//    {
//        Renderer->DrawTile(*tiles[i]);
//    }
    
    int d_fps = (int)this->fps;
    std::stringstream ss; ss << d_fps;
    Text->RenderText("fps:"+ss.str(), 200.0f, 10.0f, 0.75f, glm::vec3(1.0f,1.0f,1.0f));
}

void GameTilemap::Release()
{
    
}
