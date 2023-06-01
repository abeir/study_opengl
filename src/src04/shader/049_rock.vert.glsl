#version 450 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexCrood;
layout(location = 3) in mat4 instanceMatrix;

out vec2 vTexCrood;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vTexCrood = iTexCrood;
    gl_Position = projection * view * instanceMatrix * vec4(iPos, 1.0);
}