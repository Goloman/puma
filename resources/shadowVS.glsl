#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout(location=0) in vec3 position;

layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;

out vec3 Normal;

void main() {
	gl_Position =  projection * view * model * vec4(position, 1.0);
}
