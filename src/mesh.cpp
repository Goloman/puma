#include "mesh.hpp"

#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

puma::Mesh puma::Mesh::load(const char* filename) {
    Mesh ret;

    std::ifstream file;
    file.open(filename);
    if (file.fail() || !file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    std::string line;

    // load vertex positions
    getline(file, line);
    std::vector<glm::vec3> vertexPositions;
    int count = stoi(line);
    vertexPositions.reserve(count);
    for(int i = 0; i < count; i ++)
    {
        getline(file, line);
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>{} };
        vertexPositions.push_back(glm::vec3(stod(tokens[0]), stod(tokens[1]), stod(tokens[2])));
    }

    // load vertices
    getline(file, line);
    count = stoi(line);
    ret.positions.reserve(count);
    ret.normals.reserve(count);
    for(int i = 0; i < count; i ++)
    {
        getline(file, line);
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>{} };
        ret.positions.push_back(vertexPositions[stoi(tokens[0])]);
        ret.normals.push_back(glm::vec3(stod(tokens[1]), stod(tokens[2]), stod(tokens[3])));
    }

    //load triangles
    getline(file, line);
    count = stoi(line);
    ret.indices.reserve(count * 3);
    for(int i = 0; i < count; i ++)
    {
        getline(file, line);
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>{} };
        ret.indices.push_back(stoi(tokens[0]));
        ret.indices.push_back(stoi(tokens[1]));
        ret.indices.push_back(stoi(tokens[2]));
    }

    file.close();

    glGenVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glGenBuffers( 1, &ret.vertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, ret.vertexBuffer );

    glGenBuffers(1, &ret.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.indexBuffer);

    glBufferData(GL_ARRAY_BUFFER, (ret.positions.size()) * sizeof(glm::vec3), &ret.positions[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ret.indices.size() * sizeof(unsigned int), &ret.indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                sizeof(glm::vec3),                  // stride
                (void*)0            // array buffer offset
                );

    return ret;
}