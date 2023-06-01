#version 450 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexCrood;

out vec3 vWorldPos;
out vec3 vNormal;
out vec2 vTexCrood;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPos = model * vec4(iPos, 1.0);
    vWorldPos = worldPos.xyz;
    vNormal = mat3(transpose(inverse(model))) * iNormal;
    vTexCrood = iTexCrood;
    gl_Position = projection * view * worldPos;
}