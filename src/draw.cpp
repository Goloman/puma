#include "./Draw.h"
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>

void drawMesh(string fileWithMesh) {
	Mesh mesh = readMesh(fileWithMesh);
	vector<vec3> vertices = mesh.positions;
	vector<unsigned int> indices = mesh.indices;
	vec3* _vertices = &vertices[0];

	glBufferData(GL_ARRAY_BUFFER, (vertices.size()) * sizeof(glm::vec3), _vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
