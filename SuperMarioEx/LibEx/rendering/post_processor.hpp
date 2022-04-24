//
//  post_processor.hpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/9/26.
//

#ifndef post_processor_hpp
#define post_processor_hpp

#include "glad.h"
#include <glm/glm.hpp>

#include "texture.hpp"
#include "sprite_renderer.hpp"
#include "shader.h"

enum BindMode {
    BIND_TEXTURE2D,
    BIND_NORMAL,
    BIND_MULTISAMPLE
};

// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or
// Shake boolean.
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor
{
public:
    // State
    Shader PostProcessingShader;
    Texture2D Texture;
    GLuint Width, Height;
    // Options
    GLboolean Confuse, Chaos, Shake;
    // Constructor
    PostProcessor(Shader shader, GLuint width, GLuint height);
    // Prepares the postprocessor's framebuffer operations before rendering the game
    void BeginRender();
    // Should be called after rendering the game, so it stores all the rendered data into a texture object
    void EndRender();
    // Renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render(GLfloat time);
private:
    // Render state
    GLuint MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    GLuint RBO; // RBO is used for multisampled color buffer
    GLuint VAO;
    GLint DefaultFBO;
    int bindMode;
    // Initialize quad for rendering postprocessing texture
    void initRenderData();
    void BindFramebufferTexture2D(int width,int height);
    void BindFramebufferNormal(int width,int height);
    void BindFramebufferMultisample(int width,int height);
};

#endif /* post_processor_hpp */
