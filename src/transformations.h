#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

mat4 getModelMatrix(float angle= 0.0);
mat4 getWorldMatrix();
mat4 getProjectionMatrix(int, int);
