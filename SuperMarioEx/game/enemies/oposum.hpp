//
//  oposum.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/9.
//

#ifndef oposum_hpp
#define oposum_hpp
#include "enemy.hpp"
#include "spritesheets_animation.hpp"

class Oposum:public Enemy
{
protected:
    void DefineEnemy();
    
public:
    SpriteSheetsAnimation walk_ani;
    
    Oposum(b2World *world, glm::vec2 pos);
    void Update(float dt);
    void HitOnHead(const B2Player &player);
    void HitByEnemy(const Enemy &enemy);
    void Draw(SpriteRenderer &renderer);
};

#endif /* oposum_hpp */
