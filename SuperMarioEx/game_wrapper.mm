//
//  game_wrapper.m
//  BreakOutES
//
//  Created by chen caibin on 2021/10/29.
//

#import <Foundation/Foundation.h>
#include "game_wrapper.h"
#include <string.h>
#include "director.hpp"
#include "touch_dispatcher.hpp"
#include "color_renderer.hpp"
#include "global.hpp"
#include "game_platformer.hpp"
#include "game_b2platformer.hpp"
#include "game_mario.hpp"
#include "game_b2mario.hpp"
#include "game_tilemap.hpp"

@interface GameWrapper (){
    NSString *preferPath;
}
@end

ColorRenderer *gw_colorRenderer;

void CommonInit(){
    char vs[1024] = {0};
    char fs[1024] = {0};
    char des[1024] = {0};
    
    ResourceManager::LoadShader(Global::ResFullPath(vs,"sprite.vs"),Global::ResFullPath(fs,"sprite.fs"),
                                nullptr, "sprite");
    
    memset(vs, 0, sizeof(vs));memset(fs, 0, sizeof(fs));
    ResourceManager::LoadShader(Global::ResFullPath(vs,"particle.vs"), Global::ResFullPath(fs,"particle.fs"),
                                nullptr, "particle");
    
    memset(vs, 0, sizeof(vs));memset(fs, 0, sizeof(fs));
    ResourceManager::LoadShader(Global::ResFullPath(vs,"post_processing.vs"), Global::ResFullPath(fs,"post_processing.fs"),
                                nullptr, "postprocessing");
    
    memset(vs, 0, sizeof(vs));memset(fs, 0, sizeof(fs));
    ResourceManager::LoadShader(Global::ResFullPath(vs,"text_2d.vs"),Global::ResFullPath(fs,"text_2d.fs"),
                                nullptr, "text");
    
    memset(vs, 0, sizeof(vs));memset(fs, 0, sizeof(fs));
    ResourceManager::LoadShader(Global::ResFullPath(vs,"mesh.vs"), Global::ResFullPath(fs,"mesh.fs"),
                                nullptr, "mesh");
    
    memset(vs, 0, sizeof(vs));memset(fs, 0, sizeof(fs));
    ResourceManager::LoadShader(Global::ResFullPath(vs,"cube.vs"), Global::ResFullPath(fs,"cube.fs"),
                                nullptr, "cube");
    
    memset(vs, 0, sizeof(vs));memset(fs, 0, sizeof(fs));
    ResourceManager::LoadShader(Global::ResFullPath(vs,"mulight.vs"), Global::ResFullPath(fs,"mulight.fs"),
                                nullptr, "mulight");
    
    ResourceManager::LoadShader(Global::ResFullPath(vs,"color.vs"), Global::ResFullPath(fs,"color.fs"),
                                nullptr, "color");
    
    ResourceManager::LoadShader(Global::ResFullPath(vs,"ui.vs"), Global::ResFullPath(fs,"ui.fs"),
                                nullptr, "ui");
    
    ResourceManager::LoadShader(Global::ResFullPath(vs,"sprite.vs"),Global::ResFullPath(fs,"sprite.fs"),
                                nullptr, "tilemap");
    
    ResourceManager::LoadShader(Global::ResFullPath(vs,"b2points.vs"), Global::ResFullPath(fs,"b2points.fs"),
                                nullptr, "b2points");
    
    ResourceManager::LoadShader(Global::ResFullPath(vs,"b2lines.vs"), Global::ResFullPath(fs,"b2lines.fs"),
                                nullptr, "b2lines");

    ResourceManager::LoadShader(Global::ResFullPath(vs,"b2lines.vs"), Global::ResFullPath(fs,"b2lines.fs"),
                                nullptr, "b2triangles");
    
    ResourceManager::LoadShader(Global::ResFullPath(vs,"tilebatch.vs"),Global::ResFullPath(fs,"tilebatch.fs"),
                                nullptr, "tilebatch");
    
    // 加载纹理
    ResourceManager::LoadTexture(Global::ResFullPath(des,"background.jpg"), GL_FALSE, "background");
    ResourceManager::LoadTexture(Global::ResFullPath(des,"awesomeface.png"), GL_TRUE, "face");
    
    ResourceManager::LoadTexture(Global::ResFullPath(des,"zaitiaozhan.png"), GL_FALSE, "ui_btn_replay");
    ResourceManager::LoadTexture(Global::ResFullPath(des, "textureMask.png"), GL_TRUE, "mask");
    ResourceManager::LoadTexture(Global::ResFullPath(des, "arrow.png"), GL_TRUE, "arrow");
    ResourceManager::LoadTexture(Global::ResFullPath(des, "sheet1.png"), GL_TRUE, "sheet1.png");
    ResourceManager::LoadTexture(Global::ResFullPath(des, "sheet2.png"), GL_TRUE, "sheet2.png");
}

@implementation GameWrapper

- (void)InitWidth:(GLfloat) width Height:(GLfloat) height{
    
    preferPath = [NSString stringWithFormat:@"%@/",[[NSBundle mainBundle] bundlePath]];
    const char* fileName = [preferPath cStringUsingEncoding:1];
    
    memset(Global::RES_PATH, 0, sizeof(Global::RES_PATH));
    strcpy(Global::RES_PATH,fileName);
    Global::ScreenWidth = width;
    Global::ScreenHeight = height;
    CommonInit();
    
    GameMario::GetInstance()->Init(width, height);
    GamePlatformer::GetInstance()->Init(width, height);
    GameB2Platformer::GetInstance()->Init(width, height);
    GameB2Mario::GetInstance()->Init(width, height);
    GameTilemap::GetInstance()->Init(width, height);
    Director::GetInstance()->SetRootScene(GameB2Mario::GetInstance());
    
    Shader colorShader = ResourceManager::GetShader("color");
    gw_colorRenderer = new ColorRenderer(colorShader);
    
}

- (void)ProcessInput:(GLfloat) dt{
    
}

- (void)KeyboardInputWhithKey:(int) key Pressed:(char)pressed{
    Director::GetInstance()->GetTopScene()->KeyboardInput(key,pressed);
}

- (void)TouchBeganPosX:(double)x PosY:(double)y{
    TouchDispatcher *dispatcher = TouchDispatcher::get_instance();
    dispatcher->touchesBegan(x, y);
}

- (void)TouchEndedPosX:(double)x PosY:(double)y{
    TouchDispatcher *dispatcher = TouchDispatcher::get_instance();
    dispatcher->touchesEnded(x, y);
}

- (void)TouchMoveOffsetX:(double)x OffsetY:(double)y
{
    TouchDispatcher *dispatcher = TouchDispatcher::get_instance();
    dispatcher->touchesMovedOffset(x, y);
}

- (void)TouchMovedPosX:(double)x PosY:(double)y{
    TouchDispatcher *dispatcher = TouchDispatcher::get_instance();
    dispatcher->touchesMoved(x, y);
}

- (void)Update:(GLfloat)dt{
    Global::UpdateFrameTime();
    std::vector<IScene*> scenes = Director::GetInstance()->GetAllScenes();
    for(int i = 0; i < scenes.size();i++)
    {
        scenes[i]->Update(dt);
    }
}

- (void)Render{
    std::vector<IScene*> scenes = Director::GetInstance()->GetAllScenes();
    for(int i = 0; i < scenes.size();i++)
    {
        if(i>0){
            Shader colorShader = ResourceManager::GetShader("color");
            colorShader.use();
            colorShader.setMat4("view", glm::mat4(1.0));
            gw_colorRenderer->DrawColor({0,0,0,192}, glm::vec2(0, 0), glm::vec2(1536, 2048));
        }
        scenes[i]->Render();
    }
}

- (void)Realease
{
    std::vector<IScene*> scenes = Director::GetInstance()->GetAllScenes();
    for(int i = 0; i < scenes.size();i++)
    {
        scenes[i]->Release();
    }
}

@end
