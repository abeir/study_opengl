#version 450 core

in vec3 vTexCoord;

out vec4 FragColor;

uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, vTexCoord);
}