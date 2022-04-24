//
//  color_renderer.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/1/11.
//

#ifndef color_renderer_hpp
#define color_renderer_hpp

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.h"
#include "color.h"

class ColorRenderer
{
public:
    // Constructor (inits shaders/shapes)
    ColorRenderer(Shader &shader);
    // Destructor
    ~ColorRenderer();
    // Renders a defined color quad
    void DrawColor(Color color, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f);
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
private:
    // Render state
    Shader shader;
    GLuint quadVAO;
};

#endif /* color_renderer_hpp */
