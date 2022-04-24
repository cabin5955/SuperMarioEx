//
//  text_renderer.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/1/29.
//

#ifndef text_renderer_hpp
#define text_renderer_hpp

#include <map>

#include "glad.h"
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.hpp"
#include "shader.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};


// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:
    // Holds a list of pre-compiled Characters
    std::map<GLuint, Character> Characters;
    // Shader used for text rendering
    Shader TextShader;
    // Constructor
    TextRenderer(Shader shader, GLuint width, GLuint height);
    // Pre-compiles a list of characters from the given font
    void Load(std::string font, GLuint fontSize);
    // Renders a string of text using the precompiled list of characters
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
    // Renders a string of text using the precompiled list of characters
    void RenderText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
    
    GLfloat GetTextWidth(std::string text, GLfloat scale);
    GLfloat GetTextWidth(std::wstring text, GLfloat scale);
    
private:
    // Render state
    GLuint VAO, VBO;
    void BindTex(FT_Face &face,int c);
};

#endif /* text_renderer_hpp */
