//
//  sprite_renderer.hpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/9/25.
//

#ifndef sprite_renderer_hpp
#define sprite_renderer_hpp

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.h"
#include "color.h"
#include "mesh.hpp"
#include "tile.hpp"

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader &shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D &texture, glm::vec2 position,
                    glm::vec2 size = glm::vec2(10, 10),
                    glm::vec3 rotateAxis = glm::vec3(0.0f,0.0f,1.0f),
                    GLfloat rotateAngle = 0.0f, Color color = WHITE);
    // Initializes and configures the quad's buffer and vertex attributes
    void InitRenderData();
    
    void DrawTile(Tile &tile);
    
    void SetFlipX(bool flip);
    
    void SetFlipY(bool flip);
    
private:
    // Render state
    Shader shader;
    GLuint quadVAO;
};


#endif /* sprite_renderer_hpp */
