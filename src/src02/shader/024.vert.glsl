#version 450 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;
layout(location = 2) in vec3 iNormal;

out vec3 vNormal;
out vec3 vWordPos;
out vec2 vTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    vec4 wordPos = model * vec4(iPos, 1.0);
    vWordPos = vec3(wordPos.xyz);
    vNormal = mat3(transpose(inverse(model))) * iNormal;
    vTexCoord = iTexCoord;

    gl_Position = projection * view * vec4(vWordPos, 1.0);
}