#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float ratio;

void main() {
    FragColor = mix(texture(texture1, vTexCoord), texture(texture2, vTexCoord), ratio);
}