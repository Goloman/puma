#version 330 core
#extension GL_ARB_explicit_uniform_location : require

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (location = 12) uniform mat4 projection;

in VertexData {
    vec3 velocity;
    float age;
} gs_in[];

out float age;
out vec2 uv;
out float lineLength;

float r = 0.01f;
float len = 10.f;

float maxAge = 2.f;

void createVertex(vec3 t, vec3 b, float l, vec2 coords) {
    vec3 pos = vec3(gl_in[0].gl_Position);
    pos += t * r;
    pos += t * l/2 * r;
    pos += b * r;
    gl_Position = projection * vec4(pos, 1);
    age = gs_in[0].age / maxAge;
    uv = coords;
    lineLength = l;
    EmitVertex();
}

void main() {
    if (gs_in[0].age >= maxAge) return;

    vec3 tangent = normalize(gs_in[0].velocity);
    vec3 toCamera = vec3(-gl_in[0].gl_Position);
    vec3 binormal = normalize(cross(tangent, toCamera));

    float l = length(gs_in[0].velocity) * len;

    createVertex(tangent, binormal, l, vec2(1+l, 1));
    createVertex(tangent, -binormal, l, vec2(1+l, -1));
    createVertex(-tangent, binormal, l, vec2(-1, 1));
    createVertex(-tangent, -binormal, l, vec2(-1, -1));

    EndPrimitive();
}
