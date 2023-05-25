#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

uniform vec4 lightColor;


void main() {
    FragColor = texture(uTexture, vTexCoord) * lightColor;
}