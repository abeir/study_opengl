#version 450 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;

out vec2 oTexCoord;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(iPos, 1.0);
    oTexCoord = iTexCoord;
}