#include "Transformations.h"

mat4 getModelMatrix(float angle) {
	mat4 model;
	model = glm::rotate(model, radians(angle), vec3(1.0f, 0.0f, 0.0f));
	return model;
}
mat4 getWorldMatrix() {
	mat4 view;
	view = translate(view, vec3(0.0f, 0.0f, 0.0f));
	return view;
}
mat4 getProjectionMatrix(int screenWidth, int screenHeight) {
	mat4 projection;
	projection  = ortho(-5.0f, 5.0f, -5.0f, 5.0f, -2.0f, 2.0f);
	//projection = perspective(4.0f, (float)500 / (float)(500), -500.0f, 500.0f);
	return projection;
}