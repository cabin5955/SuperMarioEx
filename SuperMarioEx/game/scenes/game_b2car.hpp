//
//  game_b2car.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/6/11.
//

#ifndef game_b2car_hpp
#define game_b2car_hpp

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
#include "b2car.hpp"
#include "tilemap_helper.hpp"
#include <box2d/box2d.h>
#include "b2debugdraw.h"
#include "brick.hpp"
#include "coin.hpp"
#include "libex.h"

class GameB2Car:public IScene
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
    
    static GameB2Car* GetInstance()
    {
        if(s_instance == nullptr)
        {
            s_instance = new GameB2Car();
        }
        return s_instance;
    }
    
private:
    GLfloat deltaTime;
    b2World* world;
    float ZOOM = 2.0f;
    
    SpriteRenderer      *spriteRenderer;
    SpriteRenderer      *uiRenderer;
    TextRenderer        *Text;
    ColorRenderer       *colorRenderer;

    B2Car *player;
    Button *GoButton_Right,*GoButton_Left,*JumpButton;

    Camera2D camera;
    
    void UpdateCameraCenterInsideMap();
    
    GameB2Car(){}
    
    ~GameB2Car(){}

    GameB2Car(const GameB2Car&);
    GameB2Car& operator =(const GameB2Car&);
    
    static GameB2Car *s_instance;
};

#endif /* game_b2car_hpp */
