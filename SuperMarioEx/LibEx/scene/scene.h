//
//  scene.h
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/10/8.
//

#ifndef scene_h
#define scene_h

#include "glad.h"
#include "libex.h"
#include <vector>

class IScene{
public:
    GLboolean KeysProcessed[1024];
    GLboolean Keys[1024];
    double fps;
    virtual void Init(unsigned int width, unsigned int height)=0;
    virtual void OnEnter()=0;
    virtual void OnExit()=0;
    // GameLoop
    virtual void KeyboardInput(ExKeyCode keycode, ExKeyAction action)=0;
    virtual void Update(GLfloat dt)=0;
    virtual void Render()=0;
    
    virtual void Release()=0;
    int GetSceneID(){ return _sceneID; }
    
protected:
    IScene() {
        sceneID++;
        _sceneID =  sceneID;
    }
    ~IScene() {}
    unsigned int  Width, Height;

private:
    IScene(const IScene&);
    IScene& operator =(const IScene&);
    static int sceneID;
    int _sceneID;
};

#endif /* scene_h */
