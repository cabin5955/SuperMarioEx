//
//  turtle.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/18.
//

#ifndef turtle_hpp
#define turtle_hpp

#include "enemy.hpp"
#include "spritesheets_animation.hpp"

class Turtle:public Enemy
{
private:
    bool destroyed;
    bool setToDestroy;
    float stateTime;
    enum State {WALKING, MOVING_SHELL, STANDING_SHELL};
    State currentState;
    State previousState;
    
protected:
    void DefineEnemy();
    
public:
    SpriteSheetsAnimation walk_ani;
    SpriteSheetsAnimation stand_ani;
    SpriteSheetsAnimation moving_ani;
    
    Turtle(b2World *world, glm::vec2 pos);
    void Update(float dt);
    void HitOnHead(const B2Player &player);
    void HitByEnemy(const Enemy &enemy);
    void Draw(SpriteRenderer &renderer);
};


#endif /* turtle_hpp */
