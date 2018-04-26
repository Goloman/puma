#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_opengl.h>

namespace puma {

struct Mesh {
    GLuint vao;
	GLuint vaoNormals;
    GLuint vertexBuffer;
	GLuint normalsBuffer;
    GLuint indexBuffer;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    //TODO edge information
    //TODO optional texture coordinates

    static Mesh load(const char* filename);
};

}
