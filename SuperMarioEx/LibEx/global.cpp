//
//  global.cpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/3.
//

#include "global.hpp"
#include "glfm.h"
#include <string>

char Global::RES_PATH[256] = {0};
double Global::TicksSub = 0.0f;
int Global::ScreenWidth = 0;
int Global::ScreenHeight = 0;

double Global::GetTime(){
    return glfmGetTime();
}

double Global::GetFrameTime()
{
    return TicksSub;
}

double Global::UpdateFrameTime(){
    
    static double LastClickTicks = 0;
    double CurrentClickTicks;
    
    CurrentClickTicks = glfmGetTime ();
    TicksSub = CurrentClickTicks - LastClickTicks;

    /* Update LastClickTicks and signal a SingleClick. */

    LastClickTicks = CurrentClickTicks;
    
    if(TicksSub > 10) TicksSub = 0;
    
    return  TicksSub;
}

const char* Global::ResFullPath(char* des,const char *src)
{
    strcpy(des, RES_PATH);
    strcat(des, src);
    return des;
}
