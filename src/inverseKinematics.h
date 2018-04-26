#pragma once

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
namespace puma {
	void getInverseKinematics(vec3, vec3, float&, float&, float&, float&, float&);
}