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
#include "player.hpp"
#include "tilemap_helper.hpp"
#include <box2d/box2d.h>
#include "b2debugdraw.h"

class GameB2Mario:public IScene
{
public:
    
    // Initialize game state (load all shaders/textures/levels)
    void Init(unsigned int width, unsigned int height);
    void OnEnter();
    void OnExit();
    // GameLoop
    void KeyboardInput(int virtual_key, char pressed);
    void Update(GLfloat dt);
    void Render();
    
    void Release();
    
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
    b2Body* body;
    
    SpriteRenderer      *spriteRenderer;
    SpriteRenderer      *uiRenderer;
    TextRenderer        *Text;
    ColorRenderer       *colorRenderer;

    Player *player;
    Button *GoButton_Right,*GoButton_Left,*JumpButton;

    Camera2D camera;
    
    void UpdateCameraCenterInsideMap();
    
    GameB2Mario(){}
    
    ~GameB2Mario(){}

    GameB2Mario(const GameB2Mario&);
    GameB2Mario& operator =(const GameB2Mario&);
    
    static GameB2Mario *s_instance;
};

#endif /* game_b2mario_hpp */
