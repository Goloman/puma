#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (points) in;
layout (points, max_vertices = 1) out;

layout (location = 12) uniform mat4 projection;

in VertexData {
    vec3 velocity;
    float age;
} gs_in[];

out vec4 color;

void main() {
    gl_Position = projection * gl_in[0].gl_Position;
    color = vec4 (1 - gs_in[0].age / 2, gs_in[0].age / 2, 0, 1);
    EmitVertex();
    EndPrimitive();
}
