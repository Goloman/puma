#include "puma.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <fstream>

void puma::Puma::run() {
    init();
    loop();
    cleanup();
}

void puma::Puma::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        SDL_Log("%s", SDL_GetError());
        throw std::runtime_error("SDL initialization failed");
    }

    window = SDL_CreateWindow("puma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        SDL_Log("%s", SDL_GetError());
        throw std::runtime_error("Window creation failed");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0); // TODO https://wiki.libsdl.org/SDL_GLattr

    context = SDL_GL_CreateContext(window);
    if (!context) {
        SDL_Log("%s", SDL_GetError());
        throw std::runtime_error("Context creation failed");
    }

    // NOTE potentially needed?
    //glewExperimental = GL_TRUE;
    GLenum glewRet = glewInit();

    phongProgram = glCreateProgram();
    GLuint vs = createShaderFromFile("resources/phongVS.glsl", GL_VERTEX_SHADER);
    glAttachShader(phongProgram, vs);
    GLuint fs = createShaderFromFile("resources/phongFS.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(phongProgram, fs);
    glLinkProgram(phongProgram);
    int success;
    char log[1024];
    glGetProgramiv(phongProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(phongProgram, 1024, NULL, log);
        SDL_Log("%s", log);
        throw std::runtime_error("Linking failed");
    }

    robotMesh[0] = Mesh::load("resources/mesh1.txt");
    robotMesh[1] = Mesh::load("resources/mesh2.txt");
    robotMesh[2] = Mesh::load("resources/mesh3.txt");
    robotMesh[3] = Mesh::load("resources/mesh4.txt");
    robotMesh[4] = Mesh::load("resources/mesh5.txt");
    robotMesh[5] = Mesh::load("resources/mesh6.txt");

    projectiomMatrix = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 20.f);

    movingCamera = false;
    cameraPosition = {0, 0, 5};
    cameraRotationDegrees = {0, 0};

    lastTicks = SDL_GetTicks();
}

void puma::Puma::loop() {
    running = true;

    while (running) {
        handleEvents();
        update();
        render();
    }
}

void puma::Puma::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            running = false;
            } break;
        case SDL_MOUSEMOTION: {
            if (movingCamera) {
                cameraRotationDegrees += glm::vec2(event.motion.yrel, event.motion.xrel);
                cameraRotationDegrees.x = glm::clamp(cameraRotationDegrees.x, -90.f, 90.f);
                cameraRotationDegrees.y = fmod(cameraRotationDegrees.y, 360);
            }
            } break;
        case SDL_MOUSEBUTTONDOWN: {
            if (event.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                movingCamera = true;
            }
            } break;
        case SDL_MOUSEBUTTONUP: {
            if (event.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                movingCamera = false;
            }
            } break;
        default:
            break;
        }
    }
}

GLuint puma::Puma::createShaderFromFile(const char* filename, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    std::ifstream f;
    f.open(filename, std::ifstream::in | std::ifstream::binary);

    f.seekg(0, std::ios::end);
    int length = f.tellg();
    f.seekg(0, std::ios::beg);

    char* buffer = new char[length];
    f.read(buffer, length);
    f.close();

    glShaderSource(shader, 1, &buffer, &length);
    glCompileShader(shader);

    int  success;
    char log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 1024, NULL, log);
        SDL_Log("%s:\n%s", filename, log);
        throw std::runtime_error("Shader compilation failed");
    }

    delete[] buffer;
    return shader;
}

void puma::Puma::update() {
    Uint32 newTicks = SDL_GetTicks();
    dt = (newTicks - lastTicks) / 1000.f;
    lastTicks = newTicks;

    // camera update
    viewMatrix = glm::mat4(1);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotationDegrees.x), {1, 0, 0});
    viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotationDegrees.y), {0, 1, 0});
    viewMatrix = glm::translate(viewMatrix, -cameraPosition);

    glm::vec4 cameraFront = {0, 0, -1, 0};
    glm::vec4 cameraUp    = {0, 1, 0, 0};
    glm::vec4 cameraSide  = {1, 0, 0, 0};

    cameraFront = cameraFront * viewMatrix;
    cameraUp    = cameraUp    * viewMatrix;
    cameraSide  = cameraSide  * viewMatrix;

    const Uint8* keyboardState = SDL_GetKeyboardState(0);
    if (keyboardState[SDL_SCANCODE_W])
        cameraPosition += glm::vec3(cameraFront) * dt;
    if (keyboardState[SDL_SCANCODE_S])
        cameraPosition -= glm::vec3(cameraFront) * dt;
    if (keyboardState[SDL_SCANCODE_D])
        cameraPosition += glm::vec3(cameraSide) * dt;
    if (keyboardState[SDL_SCANCODE_A])
        cameraPosition -= glm::vec3(cameraSide) * dt;
    if (keyboardState[SDL_SCANCODE_LSHIFT])
        cameraPosition += glm::vec3(cameraUp) * dt;
    if (keyboardState[SDL_SCANCODE_LCTRL])
        cameraPosition -= glm::vec3(cameraUp) * dt;


    //TODO ik
    for (int i = 0; i < 6; i++) {
        robotMatrix[i] = glm::mat4(1);
    }
}

void puma::Puma::render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(phongProgram);

    glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_VIEW, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_PROJECTION, 1, GL_FALSE, glm::value_ptr(projectiomMatrix));

    for (int i = 0; i < 6; i++) {
        Mesh mesh = robotMesh[i];
        glBindVertexArray(mesh.vao);
        glEnableVertexAttribArray(SHADER_LOCATION_POSITION);
        glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_MODEL, 1, GL_FALSE, glm::value_ptr(robotMatrix[i]));
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(SHADER_LOCATION_POSITION);
    }

    SDL_GL_SwapWindow(window);
}

void puma::Puma::cleanup() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
