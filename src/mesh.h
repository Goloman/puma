#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_opengl.h>

struct Mesh {
    GLuint vao;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
};
