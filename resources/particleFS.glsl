#version 330 core

in float age;
in vec2 uv;
in float lineLength;

out vec4 outColor;

void main() {
    float d;
    if (uv.x < 0) d = length(uv);
    else if (uv.x > lineLength) d = length(uv - vec2(lineLength, 0));
    else d = abs(uv.y);

    float u = 1 - d;
    float v = 1 - age;

    float r = u + v - 1;

    if (r < 0) r = 0;
    float g = r * r;
    float b = g * g;

    g *= 0.5;
    b *= 0.1;

    outColor = vec4(r, g, b, 0.1f);
}
