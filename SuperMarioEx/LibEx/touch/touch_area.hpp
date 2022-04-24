//
//  touch_area.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/2.
//

#ifndef touch_area_hpp
#define touch_area_hpp

#include "glad.h"
#include <glm/glm.hpp>

#include "texture.hpp"
#include "color_renderer.hpp"
#include "touch_delegate_protocol.h"
#include "scene.h"
#include "button.hpp"
#include "color.h"

class TouchArea:public TouchDelegate
{
public:
    typedef void (*DoubleClickCallback)();
    typedef void (*TouchMovedOffsetCallback)(double x, double y);
    TouchArea(IScene *scene, glm::vec2 pos, glm::vec2 size, Color color);
    ~TouchArea();
    
    bool TouchBegan(double x, double y);
    // optional

    void TouchMoved(double x, double y);
    void TouchEnded(double x, double y);
    void TouchCancelled(double x, double y);
    void TouchMovedOffset(double x,double y);
    
    void Draw(ColorRenderer &renderer);

    glm::vec2   Position, Size;
    DoubleClickCallback onDoubleClick;
    TouchMovedOffsetCallback onMovedCallback;
    Color color;
    MouseState mouseState;
    
private:
    bool IsInArea;
};

#endif /* touch_area_hpp */
