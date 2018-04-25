#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec3 position;
layout (location = 3) in vec3 velocity;
layout (location = 4) in float age;

layout (location = 11) uniform mat4 view;

out VertexData {
    vec3 velocity;
    float age;
} o;

void main() {
    gl_Position = view * vec4(position, 1);
    o.velocity = vec3(view * vec4(velocity, 0));
    o.age = age;
}
