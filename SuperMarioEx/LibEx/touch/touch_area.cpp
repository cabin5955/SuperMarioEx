//
//  touch_area.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/2.
//

#include "touch_area.hpp"
#include "touch_dispatcher.hpp"
#include "global.hpp"

bool IsDoubleClick ()
{
    static double LastClickTicks = 0;
    double CurrentClickTicks;

    /* First time this function is called, LastClickTicks
    has not been initialised yet. */
    if (LastClickTicks == 0)
    {
        LastClickTicks = Global::GetTime ();
        return false;
    }
    else
    {
        CurrentClickTicks = Global::GetTime ();

        /* If the period between the two clicks is smaller
        or equal to a pre-defined number, we report a
        DoubleClick event. */
        double sub = CurrentClickTicks - LastClickTicks;
        if ( sub <= 0.4f)
        {
            /* Update LastClickTicks and signal a DoubleClick. */

            LastClickTicks = CurrentClickTicks;
            return true;
        }

        /* Update LastClickTicks and signal a SingleClick. */

        LastClickTicks = CurrentClickTicks;
        return false;
    }
}

TouchArea::TouchArea(IScene *scene, glm::vec2 pos, glm::vec2 size,Color c)
    : onDoubleClick(0)
    , onMovedCallback(0)
    , mouseState(MOUSE_NONE){
        
        Position = pos;
        Size = size;
        color = c;
        TouchDispatcher *dispatcher = TouchDispatcher::get_instance();
        dispatcher->addTargetedDelegate(scene->GetSceneID(), this, 0);
}

TouchArea::~TouchArea() {
}

bool TouchArea::TouchBegan(double x, double y) {
    //printf("touch begin!(%f,%f) pos(%f,%f),size(%f,%f)\n",x,y,Position.x,Position.y,Size.x,Size.y);
    
    if (x >= (Position.x) && x <= (Position.x+Size.x) &&
        y >= (Position.y) && y <= (Position.y+Size.y)) {
        
        if (IsDoubleClick() && onDoubleClick != 0)
        {
            onDoubleClick();
        }
    }
    return true;
}

void TouchArea::TouchEnded(double x, double y) {
    
}

void TouchArea::TouchMoved(double x, double y) {
    if (x >= (Position.x) && x <= (Position.x+Size.x) &&
        y >= (Position.y) && y <= (Position.y+Size.y)) {
        
        IsInArea = true;
    }
    else
    {
        IsInArea = false;
    }
}

void TouchArea::TouchCancelled(double x, double y)
{
    
}

void TouchArea::TouchMovedOffset(double x, double y)
{
    if(IsInArea && onMovedCallback != 0)
    {
        onMovedCallback(x,y);
    }
}

void TouchArea::Draw(ColorRenderer &renderer){
    renderer.DrawColor(color, Position, Size);
}
