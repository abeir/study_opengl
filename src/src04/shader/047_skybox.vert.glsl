#version 450 core

layout(location = 0) in vec3 iPos;

out vec3 vTexCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vTexCoord = iPos;
    vec4 pos = projection * view * vec4(iPos, 1.0);
    gl_Position = pos.xyww;
}