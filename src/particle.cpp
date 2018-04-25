#include "particle.hpp"

#include "constants.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>

void puma::ParticleSystem::init() {
    first = 0;
    count = 0;

    force = {0, -1, 0};
    startVelocity = 1.f;

    particles = new Particle[MAX_PARTICLES];
    for (size_t i = 0; i < MAX_PARTICLES; ++i) {
        particles[i].age = MAX_AGE;
    }

    srand(123);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glVertexAttribPointer(SHADER_LOCATION_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glVertexAttribPointer(SHADER_LOCATION_VELOCITY, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, velocity));
    glVertexAttribPointer(SHADER_LOCATION_AGE, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, age));
}

static inline size_t updateParticle(puma::Particle *p, float dt, glm::vec3 force, float maxAge) {
    p->age += dt;
    if (p->age >= maxAge) return 1;
    p->velocity += force * dt;
    p->position += p->velocity * dt;
    return 0;
}

void puma::ParticleSystem::update(float dt, glm::mat4 sourceMatrix) {
    size_t oldCount = 0;
    if (count) {
        size_t firstPart = MAX_PARTICLES - first;
        if (count < firstPart) firstPart = count;
        if (firstPart < count) {
            for (size_t i = 0; i < count - firstPart; i++) {
                oldCount += updateParticle(&particles[i], dt, force, MAX_AGE);
            }
            for (size_t i = first; i < MAX_PARTICLES; i++) {
                oldCount += updateParticle(&particles[i], dt, force, MAX_AGE);
            }
        } else {
            for (size_t i = first; i < first + count; i++) {
                oldCount += updateParticle(&particles[i], dt, force, MAX_AGE);
            }
        }
    }

    first = (first + oldCount) % MAX_PARTICLES;
    count -= oldCount;

    float timePerParticle = MAX_AGE / MAX_PARTICLES;
    float lastRemainder = remainder;
    remainder += dt;
    size_t newCount = remainder / timePerParticle;
    remainder = glm::mod(remainder, timePerParticle);

    size_t left = MAX_PARTICLES - count;
    if (newCount > left) newCount = left;

    for (size_t i = 0; i < newCount; i++) {
        size_t index = (i + first + count) % MAX_PARTICLES;

        float a = (rand() % 255) / 256.f;
        float b = (rand() % 255) / 256.f * glm::pi<float>() * 2.f;
        float c = (rand() % 255) / 256.f;
        c += (rand() % 255) / 256.f;
        c += (rand() % 255) / 256.f;
        c /= 3;
        c *= c;
        c = 1-c;
        c *= glm::pi<float>() / 3.f;

        glm::mat4 offset(1);
        offset = glm::rotate(offset, b, {0, 1, 0});
        offset = glm::rotate(offset, c, {0, 0, 1});

        glm::vec4 normal   = {0, 1, 0, 0};
        glm::vec4 position = {0, 0, 0, 1};
        normal = sourceMatrix * offset * normal;
        position = sourceMatrix * position;
        normal = glm::normalize(normal);

        particles[index].position = position;
        particles[index].velocity = normal * (startVelocity + a / 10.f);
        particles[index].age = 0.f;

        float epsilon = timePerParticle * (newCount - 1 - i) + lastRemainder;
        updateParticle(&particles[index], epsilon, force, MAX_AGE);
    }

    count += newCount;
}
