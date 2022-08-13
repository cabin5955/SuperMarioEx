//
//  utils.c
//  BreakoutEx
//
//  Created by chen caibin on 2022/3/4.
//

#include "utils.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Get camera 2d transform matrix
glm::mat4 GetCameraMatrix2D(Camera2D camera)
{
    glm::mat4 matTransform = glm::mat4(1.0f);
    // The camera in world-space is set by
    //   1. Move it to target
    //   2. Rotate by -rotation and scale by (1/zoom)
    //      When setting higher scale, it's more intuitive for the world to become bigger (= camera become smaller),
    //      not for the camera getting bigger, hence the invert. Same deal with rotation.
    //   3. Move it by (-offset);
    //      Offset defines target transform relative to screen, but since we're effectively "moving" screen (camera)
    //      we need to do it into opposite direction (inverse transform)

    // Having camera transform in world-space, inverse of it gives the modelview transform.
    // Since (A*B*C)' = C'*B'*A', the modelview is
    //   1. Move to offset
    //   2. Rotate and Scale
    //   3. Move by -target

    glm::mat4 translateMat= glm::translate(matTransform,{-camera.target.x, -camera.target.y, 0.0f});
    glm::mat4 rotatMat = glm::rotate(translateMat, camera.rotation*DEG2RAD, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scalMat = glm::scale(rotatMat, {camera.zoom, camera.zoom, 1.0f});
    glm::mat4 finalMat = glm::translate(scalMat,{camera.offset.x, camera.offset.y, 0.0f});
    
    return finalMat;
}

Texture2D &loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    // Create Texture object
    Texture2D *texture = new Texture2D();
    if (alpha)
    {
        texture->Internal_Format = GL_RGBA;
        texture->Image_Format = GL_RGBA;
    }
    // Load image
    int width, height, nrComponents;
    stbi_convert_iphone_png_to_rgb(1);
    unsigned char* image = stbi_load(file, &width, &height, &nrComponents, 0);
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    else
        format = GL_RED;
    
    texture->Internal_Format = format;
    texture->Image_Format = format;
    
    // Now generate texture
    texture->Generate(width, height, image);
    // And finally free image data
    stbi_image_free(image);
    return *texture;
}

glm::vec3 Vector3Transform(glm::vec3 v, glm::mat4 mat)
{
    //glm::vec4 v4(v,1.0f);
    //glm::vec4 result = v4 * mat;
    //return glm::vec3(result.x,result.y,result.z);
    
    glm::vec3 result(0.0f);
    float x = v.x;
    float y = v.y;
    float z = v.z;

    const float *pSource = (const float*)glm::value_ptr(mat);
    
    result.x = pSource[0]*x + pSource[4]*y + pSource[8]*z + pSource[12];
    result.y = pSource[1]*x + pSource[5]*y + pSource[9]*z + pSource[13];
    result.z = pSource[2]*x + pSource[6]*y + pSource[10]*z + pSource[14];

    return result;
}

// Get the screen space position for a 2d camera world space position
glm::vec2 GetWorldToScreen2D(glm::vec2 position, Camera2D camera)
{
    glm::mat4 matCamera = GetCameraMatrix2D(camera);
    glm::vec3 transform = Vector3Transform({ position.x, position.y, 0 }, matCamera);

    return (glm::vec2){ transform.x, transform.y };
}

//Modulus functions, returning only positive values
int Modulus(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

float fModulus(float a, float b)
{
    float r = fmod(a,b);
    return r < 0 ? r + b : r;
}

float Lerp(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}

glm::vec3 Lerp(glm::vec3 v0, glm::vec3 v1, float t) {
    return (glm::vec3){((1 - t) * v0.x + t * v1.x),
                     ((1 - t) * v0.y + t * v1.y),
                     ((1 - t) * v0.z + t * v1.z)};
}

glm::vec4 Lerp(glm::vec4 v0, glm::vec4 v1, float t) {
    if(t<=0) return v0;
    if(t>1) return v1;
    return {((1 - t) * v0.r + t * v1.r),
                   ((1 - t) * v0.g + t * v1.g),
                   ((1 - t) * v0.b + t * v1.b),
                   ((1 - t) * v0.a + t * v1.a)};
}

Color Lerp(Color v0, Color v1, float t) {
    if(t<=0) return v0;
    if(t>1) return v1;
    return (Color){(unsigned char)((1 - t) * v0.r + t * v1.r),
                   (unsigned char)((1 - t) * v0.g + t * v1.g),
                   (unsigned char)((1 - t) * v0.b + t * v1.b),
                   (unsigned char)((1 - t) * v0.a + t * v1.a)};
}

double Clamp(double x, double lower, double upper){
    return x>upper? upper : (x<lower? lower : x);
}

float Distance(glm::vec3 a, glm::vec3 b){
    return sqrt( (b.x-a.x)*(b.x-a.x) +
                 (b.y-a.y)*(b.y-a.y) +
                 (b.z-a.z)*(b.z-a.z));
}

int Step(float edge, float x )
{
   return x<edge? 0:1;
}

float Smoothstep(float edge0, float edge1, float x)
{
    // Scale, bias and saturate x to 0..1 range
    x = Clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}
