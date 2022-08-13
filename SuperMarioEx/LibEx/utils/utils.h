//
//  utils.h
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/4.
//

#ifndef utils_h
#define utils_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "color.h"
#include "glad.h"
#include "texture.hpp"

#define PI 3.14159265f
#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)

// Camera2D, defines position/orientation in 2d space
typedef struct Camera2D {
    glm::vec2 offset;         // Camera offset (displacement from target)
    glm::vec2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling)
} Camera2D;

glm::mat4 GetCameraMatrix2D(Camera2D camera);

// Get the screen space position for a 2d camera world space position
glm::vec2 GetWorldToScreen2D(glm::vec2 position, Camera2D camera);

// Loads a single texture from file
Texture2D &loadTextureFromFile(const GLchar *file, GLboolean alpha);

//Modulus functions, returning only positive values
int Modulus(int a, int b);
float fModulus(float a, float b);

float Lerp(float v0, float v1, float t);
glm::vec3 Lerp(glm::vec3 v0, glm::vec3 v1, float t);
glm::vec4 Lerp(glm::vec4 v0, glm::vec4 v1, float t);
Color Lerp(Color v0, Color v1, float t);

double Clamp(double x, double lower, double upper);

float Distance(glm::vec3 a, glm::vec3 b);

int Step(float edge, float x );
float Smoothstep(float edge0, float edge1, float x);

#endif /* utils_h */
