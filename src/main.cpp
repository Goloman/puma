#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "./Reader.h";

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
        gl_Position = vec4(position, 1.0);
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
	vector<vec3> vertices = readMesh("C:\\Users\\posejdon256\\Documents\\Grafika\\puma\\resources\\mesh1.txt");
	vec3* _vertices = &vertices[0];
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("puma", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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

   /* vec2 vertices[3] = {
        {-0.5, -0.5},
        {-0.5, 0.5},
        {0.5, 0},
    };*/
    GLuint vbo;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    glBufferData( GL_ARRAY_BUFFER, (vertices.size()) * sizeof(glm::vec3), _vertices, GL_STATIC_DRAW );

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    SDL_GL_SwapWindow(window);
    SDL_Delay(2000);


    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}
