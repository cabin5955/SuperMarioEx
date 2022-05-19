//
//  goomba.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/15.
//

#ifndef goomba_hpp
#define goomba_hpp

#include "enemy.hpp"
#include "spritesheets_animation.hpp"

class Goomba:public Enemy
{
private:
    bool destroyed;
    bool setToDestroy;
    float stateTime;
    
protected:
    void DefineEnemy();
    
public:
    SpriteSheetsAnimation walk_ani;
    SpriteSheetsAnimation die_ani;
    
    Goomba(b2World *world, glm::vec2 pos);
    void Update(float dt);
    void HitOnHead(const B2Player &player);
    void HitByEnemy(const Enemy &enemy);
    void Draw(SpriteRenderer &renderer);
};

#endif /* goomba_hpp */
