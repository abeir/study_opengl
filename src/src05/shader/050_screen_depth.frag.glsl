#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;


void main() {
//    FragColor = texture(screenTexture, vTexCoord);
    float depthValue = texture(screenTexture, vTexCoord).r;
    FragColor = vec4(vec3(depthValue), 1.0f);
}