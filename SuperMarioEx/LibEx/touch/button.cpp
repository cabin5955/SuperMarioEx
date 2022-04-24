//
//  button.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2021/12/23.
//

#include "button.hpp"
#include "touch_dispatcher.hpp"

Button::Button(IScene *scene, glm::vec2 pos, glm::vec2 size,Texture2D sprite, ClickCallback onClick)
    : Position(pos)
    , Size(size)
    , sprite(sprite)
    , onTouchEnded(onClick)
    , RotationAngle(0)
    , onTouchBegan(0)
    , mouseState(MOUSE_NONE){
        
        TouchDispatcher *dispatcher = TouchDispatcher::get_instance();
        dispatcher->addTargetedDelegate(scene->GetSceneID(), this, 0);
}

Button::Button(IScene *scene, glm::vec2 pos, glm::vec2 size, float rotationAngle, Texture2D sprite,
               ClickCallback touchBegan, ClickCallback touchEnded)
: Position(pos)
, Size(size)
, RotationAngle(rotationAngle)
, sprite(sprite)
, onTouchBegan(touchBegan)
, onTouchEnded(touchEnded)
, mouseState(MOUSE_NONE)
{
    TouchDispatcher *dispatcher = TouchDispatcher::get_instance();
    dispatcher->addTargetedDelegate(scene->GetSceneID(), this, 0);
}

Button::~Button() {
}

bool Button::TouchBegan(double x, double y) {
    if (x >= (Position.x) && x <= (Position.x+Size.x) &&
        y >= (Position.y) && y <= (Position.y+Size.y)) {
            mouseState = MOUSE_PRESSED;
            if(onTouchBegan !=0 ){
                onTouchBegan();
            }
        }
    return true;
}

void Button::TouchEnded(double x, double y) {
    if (x >= (Position.x) && x <= (Position.x+Size.x) &&
        y >= (Position.y) && y <= (Position.y+Size.y)
        ) {
            if(mouseState == MOUSE_PRESSED){
                if(onTouchEnded != 0){
                    onTouchEnded();
                }
            }
        }
    mouseState = MOUSE_NONE;
}

void Button::TouchMoved(double x, double y) {
    if (x >= (Position.x) && x <= (Position.x+Size.x) &&
        y >= (Position.y) && y <= (Position.y+Size.y)) {
            
        }
}

void Button::TouchCancelled(double x, double y)
{
    
}

void Button::Draw(SpriteRenderer &renderer){
    glm::vec3 axis = glm::vec3(0.0f,0.0f,1.0f);
    if(mouseState == MOUSE_PRESSED)
        renderer.DrawSprite(sprite, Position, Size*0.9f, axis, RotationAngle, WHITE);
    else
        renderer.DrawSprite(sprite, Position, Size, axis, RotationAngle, WHITE);
}
