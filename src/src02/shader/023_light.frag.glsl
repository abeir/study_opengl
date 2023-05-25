#version 450 core

out vec4 FragColor;

uniform vec4 vColor;

void main() {
    FragColor = vColor;
}