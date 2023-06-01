#version 450 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iTexCoord;

layout(std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};
uniform mat4 model;

out vec3 vWorldPos;
out vec2 vTexCoord;
out vec3 vNormal;

void main() {
    vec4 worldPos = model * vec4(iPos, 1.0);
    vWorldPos = worldPos.xyz;
    vTexCoord = iTexCoord;
    vNormal = mat3(transpose(inverse(model))) * iNormal;

    gl_Position = projection * view * worldPos;
}