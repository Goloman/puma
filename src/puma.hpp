#pragma once

#include "mesh.hpp"
#include "particle.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

namespace puma {

class Puma {
public:
    void run();

private:
    void init();
    void loop();
    void cleanup();

    void handleEvents();
    void update();
	void updateCamera();
    void render();
    void setWindowIcon();

    GLuint createShaderFromFile(const char* filename, GLenum shaderType);

    SDL_Window* window;
    SDL_GLContext context;

    glm::mat4 viewMatrix;
    glm::mat4 projectiomMatrix;

    Mesh robotMesh[6];
    glm::mat4 robotMatrix[6];
    Mesh quadMesh;

    GLuint phongProgram;
    GLuint particleProgram;

    bool running;
    bool simulating;

    Uint32 lastTicks;
    float dt;

    bool movingCamera;
    glm::vec3 cameraPosition;
    glm::vec2 cameraRotationDegrees;
	const float cameraMoveSpeed = 2.f;
	const float cameraRotationSpeed = 0.2f;

    glm::mat4 plateMatrix;
    float targetMoveRadius;
    float targetPhase;
    glm::mat4 targetMatrix;

    glm::vec3 targetNormal;
    glm::vec3 targetPosition;

    ParticleSystem particles;
	bool occludingParticles;
};

}
