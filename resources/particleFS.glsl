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
    float g = 2 * u + 4 / 3.f * v - 7 / 3.f;
    float b = 4 * u + 2 * v - 5;

    if (r < 0) r = 0;
    g = r * r;
    b = g * g;

    g *= 0.5;
    b *= 0.1;

    outColor = vec4(r, g, b, 0.1f);
    //outColor = vec4(r, g, b, 1);
    //outColor = vec4(1, g, b, 1);
}
