//
//  game_b2mario.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/13.
//

#ifndef game_b2mario_hpp
#define game_b2mario_hpp

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
#include "tmx.h"
#include "map_layer.hpp"
#include "tile.hpp"
#include "b2player.hpp"
#include "tilemap_helper.hpp"
#include <box2d/box2d.h>
#include "b2debugdraw.h"
#include "brick.hpp"
#include "coin.hpp"
#include "libex.h"

#include "subtexture.hpp"
#include "oposum.hpp"
#include "goomba.hpp"
#include "turtle.hpp"

class GameB2Mario:public IScene
{
public:
    
    // Initialize game state (load all shaders/textures/levels)
    void Init(unsigned int width, unsigned int height);
    void OnEnter();
    void OnExit();
    // GameLoop
    void KeyboardInput(ExKeyCode keycode, ExKeyAction action);
    void Update(GLfloat dt);
    void Render();
    
    void Release();
    
    void PlayerMoveForward();
    void PlayerMoveBack();
    void PlayerJump();
    void PlayerStopMove();
    
    static GameB2Mario* GetInstance()
    {
        if(s_instance == nullptr)
        {
            s_instance = new GameB2Mario();
        }
        return s_instance;
    }
    GLchar                 preferPath[1024];
    
private:
    const GLchar *FullPath(const GLchar *fileName);
    GLchar  fullPath[1024];
    GLfloat deltaTime;
    b2World* world;
    
    SpriteRenderer      *spriteRenderer;
    SpriteRenderer      *uiRenderer;
    TextRenderer        *Text;
    ColorRenderer       *colorRenderer;

    B2Player *player;
    Button *GoButton_Right,*GoButton_Left,*JumpButton;

    Camera2D camera;
    
    std::vector<Brick*> brickList;
    std::vector<Coin*> coinList;
    
    Goomba *goombas[20];
    int goombaNum;
    Turtle *turtles[20];
    int turtleNum;
    
    void UpdateCameraCenterInsideMap();
    
    GameB2Mario(){}
    
    ~GameB2Mario(){}

    GameB2Mario(const GameB2Mario&);
    GameB2Mario& operator =(const GameB2Mario&);
    
    static GameB2Mario *s_instance;
};

#endif /* game_b2mario_hpp */
