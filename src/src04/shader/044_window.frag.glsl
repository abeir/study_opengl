#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D vTexture;

void main() {
    FragColor = texture(vTexture, vTexCoord);
}