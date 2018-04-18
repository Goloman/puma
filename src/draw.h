#pragma once

#include "mesh.h"
#include "Reader.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;
using namespace glm;

void drawMesh(string fileWithMesh);
