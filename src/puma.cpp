#include "puma.hpp"

#include "constants.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>
#include <fstream>
#include <algorithm> 

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

    particleProgram = glCreateProgram();
    vs = createShaderFromFile("resources/particleVS.glsl", GL_VERTEX_SHADER);
    glAttachShader(particleProgram, vs);
    GLuint gs = createShaderFromFile("resources/particleGS.glsl", GL_GEOMETRY_SHADER);
    glAttachShader(particleProgram, gs);
    fs = createShaderFromFile("resources/particleFS.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(particleProgram, fs);
    glLinkProgram(particleProgram);
    glGetProgramiv(particleProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(particleProgram, 1024, NULL, log);
        SDL_Log("%s", log);
        throw std::runtime_error("Linking failed");
    }

    robotMesh[0] = Mesh::load("resources/mesh1.txt");
    robotMesh[1] = Mesh::load("resources/mesh2.txt");
    robotMesh[2] = Mesh::load("resources/mesh3.txt");
    robotMesh[3] = Mesh::load("resources/mesh4.txt");
    robotMesh[4] = Mesh::load("resources/mesh5.txt");
    robotMesh[5] = Mesh::load("resources/mesh6.txt");

    quadMesh = Mesh::load("resources/quad.txt");

    projectiomMatrix = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 20.f);

    movingCamera = false;
    cameraPosition = {0, 0, 5};
    cameraRotationDegrees = {0, 0};

    targetMoveRadius = .3f;
    plateMatrix = glm::mat4(1);
    plateMatrix = glm::translate(plateMatrix, {-1, 0, 0});
    plateMatrix = glm::rotate(plateMatrix, -glm::pi<float>() / 3, {0, 0, 1});
    targetPhase = 0.f;
    targetNormal = plateMatrix * glm::vec4(0, 1, 0, 0);

	particles.init();
	occludingParticles = false;

    setWindowIcon();
}

void puma::Puma::setWindowIcon() {
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    unsigned char* data = stbi_load("resources/icon.png", &width, &height, &orig_format, req_format);
    if(data == NULL) {
        SDL_Log("Loading image failed: %s", stbi_failure_reason());
    }

    // Set up the pixel format color masks for RGB(A) byte arrays.
    // Only STBI_rgb (3) and STBI_rgb_alpha (4) are supported here!
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (req_format == STBI_rgb) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (req_format == STBI_rgb) ? 0 : 0xff000000;
#endif

    int depth, pitch;
    if (req_format == STBI_rgb) {
        depth = 24;
        pitch = 3*width; // 3 bytes per pixel * pixels per row
    } else { // STBI_rgb_alpha (RGBA)
        depth = 32;
        pitch = 4*width;
    }

    SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch,
            rmask, gmask, bmask, amask);

    if (surf == NULL) {
        SDL_Log("Creating surface failed: %s", SDL_GetError());
        stbi_image_free(data);
    }

    SDL_SetWindowIcon(window, surf);

    SDL_FreeSurface(surf);
    stbi_image_free(data);
}

void puma::Puma::loop() {
    running = true;
    simulating = true;
    lastTicks = SDL_GetTicks();

    while (running) {
		Uint32 newTicks = SDL_GetTicks();
		dt = (newTicks - lastTicks) / 1000.f;
		lastTicks = newTicks;

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
                cameraRotationDegrees += glm::vec2(event.motion.yrel, event.motion.xrel) * cameraRotationSpeed;
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
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_SPACE:
                    simulating = !simulating;
                    break;
				case SDLK_TAB:
					occludingParticles = !occludingParticles;
					break;
            }
            }
            break;
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
	updateCamera();

	if (!simulating) return;

    targetPhase += dt;
    targetPhase = fmod(targetPhase, glm::pi<float>() * 2);
    targetMatrix = plateMatrix;
    targetMatrix = glm::translate(targetMatrix, {glm::sin(targetPhase) * targetMoveRadius, 0, glm::cos(targetPhase) * targetMoveRadius});
    targetMatrix = glm::scale(targetMatrix, {.1f, .1f, .1f}); // Only for drawing the debug quad, safe to remove later
    targetPosition = targetMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);

    particles.update(dt, targetMatrix);

    //TODO ik
    for (int i = 0; i < 6; i++) {
        robotMatrix[i] = glm::mat4(1);
    }
}

void puma::Puma::updateCamera() {
	viewMatrix = glm::mat4(1);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotationDegrees.x), { 1, 0, 0 });
	viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraRotationDegrees.y), { 0, 1, 0 });
	viewMatrix = glm::translate(viewMatrix, -cameraPosition);

	glm::vec4 cameraFront = { 0, 0, -1, 0 };
	glm::vec4 cameraUp = { 0, 1, 0, 0 };
	glm::vec4 cameraSide = { 1, 0, 0, 0 };

	cameraFront = cameraFront * viewMatrix;
	cameraUp = cameraUp    * viewMatrix;
	cameraSide = cameraSide  * viewMatrix;

	const Uint8* keyboardState = SDL_GetKeyboardState(0);
	if (keyboardState[SDL_SCANCODE_W])
		cameraPosition += glm::vec3(cameraFront) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_S])
		cameraPosition -= glm::vec3(cameraFront) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_D])
		cameraPosition += glm::vec3(cameraSide) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_A])
		cameraPosition -= glm::vec3(cameraSide) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_LSHIFT])
		cameraPosition += glm::vec3(cameraUp) * dt * cameraMoveSpeed;
	if (keyboardState[SDL_SCANCODE_LCTRL])
		cameraPosition -= glm::vec3(cameraUp) * dt * cameraMoveSpeed;
}

void puma::Puma::render() {
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glClearColor(0.3, 0.3, 0.3, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(phongProgram);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_VIEW, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_PROJECTION, 1, GL_FALSE, glm::value_ptr(projectiomMatrix));

    for (int i = 0; i < 6; i++) {
        Mesh mesh = robotMesh[i];
        glBindVertexArray(mesh.vao);
        glEnableVertexAttribArray(SHADER_LOCATION_POSITION);
        glEnableVertexAttribArray(SHADER_LOCATION_NORMAL);
        glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_MODEL, 1, GL_FALSE, glm::value_ptr(robotMatrix[i]));
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(SHADER_LOCATION_POSITION);
        glDisableVertexAttribArray(SHADER_LOCATION_NORMAL);
    }

        Mesh mesh = quadMesh;
        glBindVertexArray(mesh.vao);
        glEnableVertexAttribArray(SHADER_LOCATION_POSITION);
        glEnableVertexAttribArray(SHADER_LOCATION_NORMAL);
        glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_MODEL, 1, GL_FALSE, glm::value_ptr(targetMatrix));
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(SHADER_LOCATION_POSITION);
        glDisableVertexAttribArray(SHADER_LOCATION_NORMAL);

    glUseProgram(particleProgram);
    glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_VIEW, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(SHADER_UNIFORM_LOCATION_PROJECTION, 1, GL_FALSE, glm::value_ptr(projectiomMatrix));

    glBindVertexArray(particles.vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * ParticleSystem::MAX_PARTICLES, &particles.particles[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(SHADER_LOCATION_POSITION);
    glEnableVertexAttribArray(SHADER_LOCATION_VELOCITY);
    glEnableVertexAttribArray(SHADER_LOCATION_AGE);

	if (occludingParticles) {
		std::vector<unsigned int> indices;
		indices.reserve(ParticleSystem::MAX_PARTICLES);
		float *distances = new float[ParticleSystem::MAX_PARTICLES];

		for (unsigned int i = 0; i < ParticleSystem::MAX_PARTICLES; ++i) {
			glm::vec3 offset = particles.particles[i].position - cameraPosition;
			distances[i] = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
			indices.push_back(i);
		}

		std::sort(indices.begin(), indices.end(), [distances](unsigned int a, unsigned int b) {
			return (distances[a] > distances[b]);
		});

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ParticleSystem::MAX_PARTICLES * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
	}

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    

	if (occludingParticles) {
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDrawElements(GL_POINTS, ParticleSystem::MAX_PARTICLES, GL_UNSIGNED_INT, 0);
	} else {
		glBlendFunc(GL_ONE, GL_ONE);
		glDrawArrays(GL_POINTS, 0, ParticleSystem::MAX_PARTICLES);
	}


    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    glDisableVertexAttribArray(SHADER_LOCATION_AGE);
    glDisableVertexAttribArray(SHADER_LOCATION_VELOCITY);
    glDisableVertexAttribArray(SHADER_LOCATION_POSITION);

    SDL_GL_SwapWindow(window);
}

void puma::Puma::cleanup() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
