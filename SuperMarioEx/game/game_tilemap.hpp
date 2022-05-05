//
//  game_tilemap.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/27.
//

#ifndef game_tilemap_hpp
#define game_tilemap_hpp

#include "glad.h"
#include <vector>
#include <string>
#include "scene.h"
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

class GameTilemap:public IScene
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
    
    static GameTilemap* GetInstance()
    {
        if(s_instance == nullptr)
        {
            s_instance = new GameTilemap();
        }
        return s_instance;
    }
    GLchar                 preferPath[1024];
    
private:
    const GLchar *FullPath(const GLchar *fileName);
    GLchar  fullPath[1024];
    GLfloat deltaTime;
    
    SpriteRenderer      *spriteRenderer;
    SpriteRenderer      *uiRenderer;
    TextRenderer        *Text;
    ColorRenderer       *colorRenderer;

    Camera2D camera;
    
    GameTilemap(){}
    
    ~GameTilemap(){}

    GameTilemap(const GameTilemap&);
    GameTilemap& operator =(const GameTilemap&);
    
    static GameTilemap *s_instance;
};


#endif /* game_tilemap_hpp */
