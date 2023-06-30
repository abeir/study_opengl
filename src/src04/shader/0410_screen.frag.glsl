#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;


void main() {
    FragColor = texture(screenTexture, vTexCoord);
}