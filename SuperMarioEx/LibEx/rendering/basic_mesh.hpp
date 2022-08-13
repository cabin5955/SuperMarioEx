//
//  basic_mesh.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/7/9.
//

#ifndef basic_mesh_hpp
#define basic_mesh_hpp
#include "glad.h"
#include "basic_vertex.h"
#include "shader.h"
#include "texture.hpp"

class BasicMesh
{
public:
    BasicMesh(Shader &shader, BasicVertex *vertices, unsigned int vsize, int *indices,unsigned int isize);
    BasicMesh(Shader &shader, BasicVertex *vertices, unsigned int vsize, int *indices,unsigned int isize, bool calcNormals);
    void Draw(Texture2D &texture, glm::vec3 position, glm::vec3 size = glm::vec3(1, 1, 1),
              float rotation= 0.0f, glm::vec3 axis = glm::vec3(0, 0, 1.0f));
private:
    void InitMeshData();
    void AddVertices(BasicVertex *vertices, unsigned int vsize, int *indices,unsigned int isize, bool calcNormals);
    void CalcNormals(BasicVertex *vertices, unsigned int vsize, int *indices,unsigned int isize);
    unsigned int vao,vbo,ibo;
    unsigned int size;
    
    BasicVertex *vertices;
    int *indices;
    
    Shader shader;
};

#endif /* basic_mesh_hpp */
