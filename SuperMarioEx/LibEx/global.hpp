//
//  global.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/3.
//

#ifndef global_hpp
#define global_hpp

#define PPM 100.0f

class Global{
public:
    static char RES_PATH[256];
    static double GetTime();
    static double GetFrameTime();
    static const char* ResFullPath(char* des,const char *src);
    static double UpdateFrameTime();
    
    static int ScreenWidth;
    static int ScreenHeight;
    
private:
    static double TicksSub;
};

#endif /* global_hpp */
