#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "transformations.h"
#include "Reader.h"
#include "draw.h"

using namespace glm;
using namespace std;

const char* vertexSource = R"glsl(
    #version 150 core
    in vec3 position;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
    void main()
    {
        gl_Position =  projection * view * model  * vec4(position, 1.0);
        gl_Position =  model  * vec4(position, 1.0);
    }
)glsl";

const char* fragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)glsl";

int main() {

	// get vertices from mesh	
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("puma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(getModelMatrix()));
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(getWorldMatrix()));
    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(getProjectionMatrix(1000, 1000)));

    Mesh m1 = readMesh("resources/mesh1.txt");
    Mesh m2 = readMesh("resources/mesh2.txt");
    Mesh m3 = readMesh("resources/mesh3.txt");
    Mesh m4 = readMesh("resources/mesh4.txt");
    Mesh m5 = readMesh("resources/mesh5.txt");
    Mesh m6 = readMesh("resources/mesh6.txt");

    bool running = true;
    while (running) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            /* handle your event here */
        }
        float angle = SDL_GetTicks() / 10.0f;
        modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(getModelMatrix(angle)));
        drawMesh(m1);
        drawMesh(m2);
        drawMesh(m3);
        drawMesh(m4);
        drawMesh(m5);
        drawMesh(m6);
        SDL_GL_SwapWindow(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}
