#version 450 core

out vec4 FragColor;

uniform vec4 vertColor;

void main() {
    FragColor = vertColor;
}