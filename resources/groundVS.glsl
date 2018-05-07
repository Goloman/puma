#version 330 core
#extension GL_ARB_explicit_uniform_location : require


layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

layout(location = 10) uniform mat4 model;
layout(location = 11) uniform mat4 view;
layout(location = 12) uniform mat4 projection;

out vec2 TexCoord;

out vec3 FragPos; 
out vec3 Normal;

void main() {
    gl_Position =  projection * view * model  * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(model))) * normal;  
	TexCoord = vec2((position.x + 0.5f), (position.z + 0.5f));
}
