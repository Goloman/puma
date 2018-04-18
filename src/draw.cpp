#include "draw.h"
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>

void drawMesh(Mesh mesh) {
    glBindVertexArray(mesh.vao);
    glEnableVertexAttribArray(0);
    glBindBuffer( GL_ARRAY_BUFFER, mesh.vertexBuffer );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer);
glVertexAttribPointer(
   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   3,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
}
