#version 330 core

in float age;
in vec2 uv;
in float lineLength;

out vec4 outColor;

void main() {
    float r;
    if (uv.x < 0) r = length(uv);
    else if (uv.x > lineLength) r = length(uv - vec2(lineLength, 0));
    else r = abs(uv.y);
    r = floor(r * 2);
    outColor = vec4(1-age, age, 1-r, 1-r);
}
