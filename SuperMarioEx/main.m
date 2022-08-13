// Example app that draws a triangle. The triangle can be moved via touch or keyboard arrow keys.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_COMPAT_ANDROID_ACTIVITY glfmAndroidGetActivity()
#include "file_compat.h"
#include "glfm.h"
#include "game_wrapper.h"
#include "LibEx/libex.h"

typedef struct {

    double lastTouchX;
    double lastTouchY;

    double offsetX;
    double offsetY;

    bool needsRedraw;
} ExampleApp;

static void onFrame(GLFMDisplay *display);
static void onSurfaceCreated(GLFMDisplay *display, int width, int height);
static void onSurfaceRefresh(GLFMDisplay *display);
static void onSurfaceDestroyed(GLFMDisplay *display);
static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y);
static bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers);

GameWrapper *wrapper;

double GetDeltaTime ()
{
    static double LastClickTicks = 0;
    double CurrentClickTicks;

    CurrentClickTicks = glfmGetTime ();
    double sub = CurrentClickTicks - LastClickTicks;

    /* Update LastClickTicks and signal a SingleClick. */

    LastClickTicks = CurrentClickTicks;
    return  sub;
}


// Main entry point
void glfmMain(GLFMDisplay *display) {
    ExampleApp *app = calloc(1, sizeof(ExampleApp));
    wrapper = [[GameWrapper alloc] init];
    glfmSetDisplayConfig(display,
                         GLFMRenderingAPIOpenGLES3,
                         GLFMColorFormatRGBA8888,
                         GLFMDepthFormat16,
                         GLFMStencilFormatNone,
                         GLFMMultisampleNone);
    glfmSetUserData(display, app);
    glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
    glfmSetSurfaceRefreshFunc(display, onSurfaceRefresh);
    glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
    glfmSetRenderFunc(display, onFrame);
    glfmSetTouchFunc(display, onTouch);
    glfmSetKeyFunc(display, onKey);
}

static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y) {
    if (phase == GLFMTouchPhaseHover) {
        return false;
    }
    ExampleApp *app = glfmGetUserData(display);
    app->needsRedraw = true;
    if(phase == GLFMTouchPhaseBegan)
    {
        [wrapper TouchBeganPosX:x PosY:y];
    }
    else if(phase == GLFMTouchPhaseEnded)
    {
        [wrapper TouchEndedPosX:x PosY:y];
    }
    else if(phase == GLFMTouchPhaseMoved)
    {
        [wrapper TouchMovedPosX:x PosY:y];
    }
    
    if (phase != GLFMTouchPhaseBegan)
    {
        int width, height;
        glfmGetDisplaySize(display, &width, &height);
        app->offsetX =  x - app->lastTouchX;
        app->offsetY =  y - app->lastTouchY;
        [wrapper TouchMoveOffsetX:app->offsetX OffsetY:app->offsetY];
    }
    app->lastTouchX = x;
    app->lastTouchY = y;
    return true;
}

static bool onKey(GLFMDisplay *display, GLFMKey keyCode, GLFMKeyAction action, int modifiers) {
    bool handled = false;
    [wrapper KeyboardInputWhithKey:(ExKeyCode)keyCode Pressed:(ExKeyAction)action];
    
    if (action == GLFMKeyActionPressed) {
        ExampleApp *app = glfmGetUserData(display);
        app->needsRedraw = true;
        switch (keyCode) {
            case GLFMKeyLeft:
                app->offsetX -= 0.1f;
                handled = true;
                break;
            case GLFMKeyRight:
                app->offsetX += 0.1f;
                handled = true;
                break;
            case GLFMKeyUp:
                app->offsetY += 0.1f;
                handled = true;
                break;
            case GLFMKeyDown:
                app->offsetY -= 0.1f;
                handled = true;
                break;
            default:
                break;
        }
    }
    return handled;
}

static void onSurfaceCreated(GLFMDisplay *display, int width, int height) {
    GLFMRenderingAPI api = glfmGetRenderingAPI(display);
    printf("Hello from GLFM! Using OpenGL %s\n",
           api == GLFMRenderingAPIOpenGLES32 ? "ES 3.2" :
           api == GLFMRenderingAPIOpenGLES31 ? "ES 3.1" :
           api == GLFMRenderingAPIOpenGLES3 ? "ES 3.0" : "ES 2.0");
    [wrapper InitWidth:width Height:height];
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void onSurfaceRefresh(GLFMDisplay *display) {
    ExampleApp *app = glfmGetUserData(display);
    app->needsRedraw = true;
}

static void onSurfaceDestroyed(GLFMDisplay *display) {
    // When the surface is destroyed, all existing GL resources are no longer valid.
    
}

static void draw(ExampleApp *app, int width, int height) {
    
    // Draw background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    [wrapper Render];
}

static void onFrame(GLFMDisplay *display) {
    ExampleApp *app = glfmGetUserData(display);
    //if (app->needsRedraw)
    {
        app->needsRedraw = false;
        
        [wrapper Update:GetDeltaTime()];
        
        int width, height;
        glfmGetDisplaySize(display, &width, &height);
        draw(app, width,  height);
        glfmSwapBuffers(display);
    }
}

