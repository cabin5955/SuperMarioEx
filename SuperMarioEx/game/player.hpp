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

#define G 400
#define PLAYER_JUMP_SPD 350.0f

typedef struct EnvItem {
    glm::vec2 position;
    glm::vec2 size;
    int blocking;
    Color color;
    char name[20];
} EnvItem;

class Player:public GameObject {
public:
    
    bool isWalk;
    bool isJump;
    float timerAnimation;
    float speed;
    bool canJump;
    bool flipX;
    
    SpritesAnimation idle_ani;
    SpritesAnimation climb_ani;
    SpritesAnimation hurt_ani;
    SpritesAnimation jump_ani;
    SpritesAnimation run_ani;
    SpritesAnimation crouch_ani;
    
    Player(glm::vec2 pos);
    void Update(EnvItem *envItems, int envItemsLength);
    void Update();
    void Draw(SpriteRenderer &renderer) override;
};


#endif /* player_hpp */
