//
//  game_platformer.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/3.
//

#ifndef game_b2platformer_hpp
#define game_b2platformer_hpp

#include "glad.h"
#include <vector>
#include "scene.h"
#include <string>
#include <box2d/box2d.h>
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "text_renderer.hpp"
#include "color_renderer.hpp"
#include "director.hpp"
#include "button.hpp"
#include "color.h"
#include "global.hpp"
#include "utils.h"
#include "mesh.hpp"
#include "b2debugdraw.h"
#include "libex.h"

class GameB2Platformer:public IScene
{
public:
    typedef struct Player {
        glm::vec2 position;
        glm::vec2 size;
        float speed;
        bool canJump;
    } Player;

    typedef struct EnvItem {
        glm::vec2 position;
        glm::vec2 size;
        int blocking;
        Color color;
    }EnvItem;
    
    // Initialize game state (load all shaders/textures/levels)
    void Init(unsigned int width, unsigned int height);
    void OnEnter();
    void OnExit();
    // GameLoop
    void KeyboardInput(ExKeyCode keycode, ExKeyAction action);
    void Update(GLfloat dt);
    void Render();
    
    void Release();
    
    static GameB2Platformer* GetInstance()
    {
        if(s_instance == nullptr)
        {
            s_instance = new GameB2Platformer();
        }
        return s_instance;
    }
    
private:
    const GLchar *FullPath(const GLchar *fileName);
    GLchar  fullPath[1024];
    GLfloat deltaTime;
    b2World* world;
    b2Body* body;
    b2Body* wheel;
    b2RevoluteJoint* joint;
    SpriteRenderer      *spriteRenderer;
    SpriteRenderer      *uiRenderer;
    ColorRenderer       *colorRenderer;

    Player player;
    Button *GoButton_Right,*GoButton_Left,*JumpButton;
    Camera2D camera;
    
    GameB2Platformer(){}
    
    ~GameB2Platformer(){}

    GameB2Platformer(const GameB2Platformer&);
    GameB2Platformer& operator =(const GameB2Platformer&);
    
    static GameB2Platformer *s_instance;
};

#endif
