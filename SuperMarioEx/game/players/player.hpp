//
//  player.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/4/4.
//

#ifndef player_hpp
#define player_hpp

#include "sprites_animation.hpp"
#include "game_object.hpp"
#include <vector>

#define G 400
#define PLAYER_JUMP_SPD 350.0f

typedef struct EnvItem {
    glm::vec2 position;
    glm::vec2 size;
    int blocking;
    Color color;
    char name[20];
} EnvItem;

typedef struct MultPointsItem{
    std::vector<glm::vec2> vertices;
}MultPointsItem;

class Player:public GameObject {
public:
    
    bool isWalk;
    bool isJump;
    float timerAnimation;
    float speed;
    bool canJump;
    bool flipX;
    int hitTimes;
    
    SpritesAnimation idle_ani;
    SpritesAnimation climb_ani;
    SpritesAnimation hurt_ani;
    SpritesAnimation jump_ani;
    SpritesAnimation run_ani;
    SpritesAnimation crouch_ani;
    
    static const short NOTHING_BIT = 0;
    static const short GROUND_BIT = 1;
    static const short MARIO_BIT = 2;
    static const short BRICK_BIT = 4;
    static const short COIN_BIT = 8;
    static const short DESTROYED_BIT = 16;
    static const short OBJECT_BIT = 32;
    static const short ENEMY_BIT = 64;
    static const short ENEMY_HEAD_BIT = 128;
    static const short ITEM_BIT = 256;
    static const short MARIO_HEAD_BIT = 512;
    static const short FIREBALL_BIT = 1024;
    static const short PLATFORM_BIT = 2048;
    
    Player(glm::vec2 pos);
    void Update(EnvItem *envItems, int envItemsLength);
    void Update();
    void Draw(SpriteRenderer &renderer) override;
    void Draw(SpriteRenderer &renderer, glm::vec2 &pos);
};


#endif /* player_hpp */
