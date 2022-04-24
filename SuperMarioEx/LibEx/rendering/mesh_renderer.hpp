//
//  MeshRenderer.hpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/10/7.
//

#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "shader.h"
#include "mesh.hpp"
#include "model.hpp"
#include "color.h"

class MeshRenderer
{
public:
    // Constructor (inits shaders/shapes)
    MeshRenderer(Shader &shader,float vertics[],size_t vsize,
                 float texcoords[],size_t tsize,float normals[],size_t nsize);
    
    MeshRenderer(Shader &shader,
                 std::vector<glm::vec3> & out_vertices,
                 std::vector<glm::vec2> & out_uvs,
                 std::vector<glm::vec3> & out_normals);
    
    // Destructor
    ~MeshRenderer();
    // Renders a defined quad textured with given sprite
    void DrawMesh(Texture2D &texture, glm::vec3 position, glm::vec3 size = glm::vec3(1, 1, 1),
                  GLfloat angle = 0.0f, glm::vec3 axis = glm::vec3(0, 0, 1.0f),
                  Color color = WHITE);
    
    void DrawMesh(Texture2D &texture, glm::vec3 position,
                  glm::vec3 size = glm::vec3(1, 1, 1),
                  glm::quat q = glm::quat(1.f,0.0f,0.0f,0.0f),
                  Color color = WHITE);
    
    static void DrawMesh(Shader &shader,Model &model,Texture2D &texture,
                         glm::vec3 position,glm::vec3 size = glm::vec3(1, 1, 1),
                         GLfloat angle = 0.0f, glm::vec3 axis = glm::vec3(0, 0, 1.0f),
                         Color color = WHITE);
    
private:
    // Render state
    Shader shader;
    GLuint meshVAO;
    GLuint verticsSize;
    
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint uvbuffer;
    
    void Init(float *vertics,size_t vSize,float *texcoords,size_t tSize,float *normals,size_t nSize);
};

#endif /* MeshRenderer_hpp */
