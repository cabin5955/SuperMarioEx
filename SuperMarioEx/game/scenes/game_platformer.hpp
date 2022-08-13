//
//  game_platformer.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/3.
//

#ifndef game_platformer_hpp
#define game_platformer_hpp

#include "glad.h"
#include <vector>
#include "scene.h"
#include <string>
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "text_renderer.hpp"
#include "director.hpp"
#include "button.hpp"
#include "color_renderer.hpp"
#include "color.h"
#include "global.hpp"
#include "utils.h"
#include "mesh.hpp"
#include "libex.h"

class GamePlatformer:public IScene
{
public:
    
    typedef struct Player {
        glm::vec2 position;
        float speed;
        bool canJump;
    } Player;

    typedef struct EnvItem {
        glm::vec2 position;
        glm::vec2 size;
        int blocking;
        Color color;
    } EnvItem;
    
    // Initialize game state (load all shaders/textures/levels)
    void Init(unsigned int width, unsigned int height);
    void OnEnter();
    void OnExit();
    // GameLoop
    void KeyboardInput(ExKeyCode keycode, ExKeyAction action);
    void Update(GLfloat dt);
    void Render();
    
    void Release();
    
    static GamePlatformer* GetInstance()
    {
        if(s_instance == nullptr)
        {
            s_instance = new GamePlatformer();
        }
        return s_instance;
    }
    GLchar                 preferPath[1024];
    
private:
    const GLchar *FullPath(const GLchar *fileName);
    GLchar  fullPath[1024];
    GLfloat deltaTime;
    
    SpriteRenderer      *uiRenderer;
    ColorRenderer       *colorRenderer;
    
    TextRenderer        *Text;

    Player player;
    Button *GoButton_Right,*GoButton_Left,*JumpButton;
    
    Camera2D camera;
    
    GamePlatformer(){}
    
    ~GamePlatformer(){}

    GamePlatformer(const GamePlatformer&);
    GamePlatformer& operator =(const GamePlatformer&);
    
    static GamePlatformer *s_instance;
};

#endif /* game_platformer_hpp */
