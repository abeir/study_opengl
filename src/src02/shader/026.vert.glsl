#version 450 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;
layout(location = 2) in vec3 iNormal;

out vec3 vWorldPos;
out vec2 vTexCoord;
out vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPos = model * vec4(iPos, 1.0);
    vWorldPos = worldPos.xyz;
    vTexCoord = iTexCoord;
    vNormal = mat3(transpose(inverse(model))) * iNormal;

    gl_Position = projection * view * worldPos;
}