#pragma once

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

namespace puma {

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float age;
};

struct ParticleSystem {
    static const size_t MAX_PARTICLES = 1024;
    constexpr static const float MAX_AGE = 2.f;

    GLuint vao;

    Particle particles[MAX_PARTICLES];
    size_t first;
    size_t count;

    float remainder;

    glm::vec3 force;
    float startVelocity;

    void init();
    void update(float dt, glm::mat4 sourceMatrix);
};

}
