//
//  mesh.hpp
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/6.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define LINE_SIZE 256
#define PATH_SIZE 256

#define UNUSED_VAR(x) ((void)(x))

#define DARRAY_RAW_DATA(darray) ((int*)(darray) - 2)
#define DARRAY_CAPACITY(darray) (DARRAY_RAW_DATA(darray)[0])
#define DARRAY_OCCUPIED(darray) (DARRAY_RAW_DATA(darray)[1])

template <typename T>
int darray_size(T *darray) {
    return darray != NULL ? DARRAY_OCCUPIED(darray) : 0;
}

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
    glm::vec4 tangent;
    glm::vec4 joint;
    glm::vec4 weight;
} Vertex;

typedef struct {
    int num_faces;
    Vertex *vertices;
    glm::vec3 center;
} Mesh;

typedef struct Rectangle {
    float x;                // top-left corner position x
    float y;                // top-left corner position y
    float width;
    float height;           
} Rectangle;

/* mesh loading/releasing */
Mesh *mesh_load(const char *filename);
void mesh_release(Mesh *mesh);

/* vertex retrieving */
int mesh_get_num_faces(Mesh *mesh);
Vertex *mesh_get_vertices(Mesh *mesh);
glm::vec3 mesh_get_center(Mesh *mesh);

#endif /* mesh_hpp */
