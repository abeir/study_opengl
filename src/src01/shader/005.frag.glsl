#version 450 core

in vec4 oColor;
in vec2 oTexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float ratio;

void main() {
    FragColor = mix(texture(texture1, oTexCoord), texture(texture2, vec2(1.0-oTexCoord.x, oTexCoord.y)), ratio) * oColor;
}