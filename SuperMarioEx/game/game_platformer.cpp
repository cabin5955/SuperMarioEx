//
//  game_platformer.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/3.
//

#include "game_platformer.hpp"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

GamePlatformer* GamePlatformer::s_instance = nullptr;

GamePlatformer::EnvItem envItems[] = {
    {{ 0, -400 }, { 1000, 800 }, 0, LIGHTGRAY },
    {{ 0, 400 }, { 1000, 200 }, 4, GRAY },
    {{ 300, 200 }, { 400, 10 }, 4, GRAY },
    {{ 250, 300 }, { 100, 10 }, 4, GRAY },
    {{ 650, 300 }, { 100, 10 }, 4, GRAY },
    {{ 300, 100 }, { 200, 10 }, 4, GRAY },
    {{ 200, 0   }, { 100, 10 }, 4, GRAY },
    {{ 250, -100 },{ 100, 10 }, 4, GRAY }
};

int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

void GoLeft()
{

}

void GoRight()
{
    
}

void Jump()
{
    
}

void UpdatePlayer(GamePlatformer::Player *player, GamePlatformer::EnvItem *envItems, int envItemsLength, float delta)
{
    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++)
    {
        GamePlatformer::EnvItem *ei = envItems + i;
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


void UpdateCameraCenter(Camera2D *camera, GamePlatformer::Player *player, int width, int height);

void UpdateCameraCenterInsideMap(Camera2D *camera, GamePlatformer::Player *player, GamePlatformer::EnvItem *envItems, int envItemsLength, int width, int height);

void UpdateCameraCenterSmoothFollow(Camera2D *camera, GamePlatformer::Player *player, float delta, int width, int height);

void GamePlatformer::Init(unsigned int width, unsigned int height)
{
    this->Width = width;
    this->Height = height;
    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                        static_cast<GLfloat>(this->Height),0.0f,
                                        -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("view", glm::mat4(1.0));
    spriteShader.setMat4("projection", projection);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", glm::mat4(1.0));
    colorShader.setMat4("projection", projection);
    
    Shader uiShader = ResourceManager::GetShader("ui");
    uiShader.use();
    uiShader.setMat4("projection", projection);
    
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
                                GoRight,0);
    
    GoButton_Left = new Button(this,glm::vec2(this->Width/2-btn_size*scal-btn_size*4,this->Height-128*scal),
                               glm::vec2(btn_size*scal,btn_size*scal),
                               180.0f*DEG2RAD,
                               ResourceManager::GetTexture("arrow"),
                               GoLeft,0);
    
    JumpButton = new Button(this,glm::vec2(this->Width-btn_size*4,this->Height-128*scal),
                            glm::vec2(btn_size*scal,btn_size*scal),
                            -90.0f*DEG2RAD,
                            ResourceManager::GetTexture("arrow"),
                            Jump,0);
    
    player.position = { 400, 280 };
    player.speed = 0;
    player.canJump = false;
    
    camera.target = player.position;
    camera.offset = { width/2.0f, height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void GamePlatformer::OnEnter(){
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                        static_cast<GLfloat>(this->Height),0.0f,
                                        -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("projection", projection);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("projection", projection);
}

void GamePlatformer::OnExit(){
    
}

void GamePlatformer::KeyboardInput(ExKeyCode keycode, ExKeyAction action)
{
    
}

void GamePlatformer::Update(GLfloat dt)
{
    float deltaTime = Global::GetFrameTime();
    if(GoButton_Right->mouseState == MOUSE_PRESSED)
    {
        player.position.x += PLAYER_HOR_SPD*deltaTime;
    }
    else if(GoButton_Left->mouseState == MOUSE_PRESSED)
    {
        float step = PLAYER_HOR_SPD*deltaTime;
        player.position.x -= step;
    }
    
    if(JumpButton->mouseState == MOUSE_PRESSED)
    {
        if (player.canJump)
        {
            player.speed = -PLAYER_JUMP_SPD;
            player.canJump = false;
        }
    }
    
    UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
    
    //UpdateCameraCenter(&camera, &player, this->Width, this->Height);
    UpdateCameraCenterInsideMap(&camera, &player, envItems, envItemsLength, this->Width, this->Height*0.6f);
    //UpdateCameraCenterSmoothFollow(&camera, &player, deltaTime, this->Width, this->Height);
}

void GamePlatformer::Render()
{
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", GetCameraMatrix2D(camera));
    
    for (int i = 0; i < envItemsLength; i++)
    {
        colorRenderer->DrawColor(envItems[i].color, envItems[i].position,envItems[i].size);
    }

    colorRenderer->DrawColor( ORANGE,glm::vec2(player.position.x - 20, player.position.y - 40), {40, 40});
    
    GoButton_Left->Draw(*uiRenderer);
    GoButton_Right->Draw(*uiRenderer);
    JumpButton->Draw(*uiRenderer);
}

void GamePlatformer::Release()
{
    
}

void UpdateCameraCenter(Camera2D *camera, GamePlatformer::Player *player, int width, int height)
{
    camera->offset = { width/2.0f, height/2.0f };
    camera->target = player->position;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, GamePlatformer::Player *player, GamePlatformer::EnvItem *envItems, int envItemsLength, int width, int height)
{
    camera->target = player->position;
    camera->offset = { width/2.0f, height/2.0f };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++)
    {
        GamePlatformer::EnvItem *ei = envItems + i;
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

void UpdateCameraCenterSmoothFollow(Camera2D *camera, GamePlatformer::Player *player, float delta, int width, int height)
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
