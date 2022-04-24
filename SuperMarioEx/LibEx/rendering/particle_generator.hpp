//
//  particle_generator.hpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/9/26.
//

#ifndef particle_generator_hpp
#define particle_generator_hpp

#include <vector>

#include "glad.h"
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.hpp"
#include "game_object.hpp"


// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
    // Update all particles
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
    // Render all particles
    void Draw();
private:
    // State
    std::vector<Particle> particles;
    GLuint amount;
    // Render state
    Shader shader;
    Texture2D texture;
    GLuint VAO;
    // Initializes buffer and vertex attributes
    void init();
    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    GLuint firstUnusedParticle();
    // Respawns particle
    void respawnParticle(Particle &particle, GameObject &object, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};

#endif /* particle_generator_hpp */
