#version 450 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexCoord;

out vec2 vTexCoord;

void main() {
    vTexCoord = iTexCoord;
    gl_Position = vec4(iPos, 1.0, 1.0);
}