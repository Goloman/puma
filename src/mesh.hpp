#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_opengl.h>

namespace puma {

const int SHADER_LOCATION_POSITION = 0;
const int SHADER_LOCATION_NORMAL = 1;
const int SHADER_LOCATION_TEXTURE_COORDINATE = 2;
const int SHADER_LOCATION_VELOCITY = 3;
const int SHADER_LOCATION_AGE = 4;

const int SHADER_UNIFORM_LOCATION_MODEL = 10;
const int SHADER_UNIFORM_LOCATION_VIEW = 11;
const int SHADER_UNIFORM_LOCATION_PROJECTION = 12;
const int SHADER_UNIFORM_LOCATION_LIGHT_POSITION = 13;

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
