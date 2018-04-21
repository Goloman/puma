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
            break;
            }
        default:
            break;
        }
    }
}

GLuint puma::Puma::createShaderFromFile(const char* filename, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    std::ifstream f;
    f.open(filename);

    f.seekg(0, std::ios::end);
    int length = f.tellg();
    f.seekg(0, std::ios::beg);

    char* buffer = new char[length];
    f.read(buffer, length);
    f.close();

    //char *start = buffer;
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
    //TODO ik
    for (int i = 0; i < 6; i++) {
        robotMatrix[i] = glm::mat4(1);
    }

    viewMatrix = glm::translate(glm::mat4(1), {0, 0, -5});
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
