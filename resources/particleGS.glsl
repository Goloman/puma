#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (location = 12) uniform mat4 projection;

in VertexData {
    vec3 velocity;
    float age;
} gs_in[];

out vec4 color;

float r = 0.002f;
float len = 0.01f;

void createVertex(vec3 t, vec3 b, float l) {
    vec3 pos = vec3(gl_in[0].gl_Position);
    pos += t * r;
    pos += t * l;
    pos += b * r;
    gl_Position = projection * vec4(pos, 1);
    color = vec4 (1 - gs_in[0].age / 2, gs_in[0].age / 2, 0, 1);
    EmitVertex();
}

void main() {
    vec3 tangent = normalize(gs_in[0].velocity);
    vec3 toCamera = vec3(-gl_in[0].gl_Position);
    vec3 binormal = normalize(cross(tangent, toCamera));

    float l = length(gs_in[0].velocity) * len;

    createVertex(tangent, binormal, l);
    createVertex(tangent, -binormal, l);
    createVertex(-tangent, binormal, l);
    createVertex(-tangent, -binormal, l);

    EndPrimitive();
}
