#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D vTexture;

void main() {
    vec4 texColor = texture(vTexture, vTexCoord);
    if(texColor.a < 0.1){
        discard;
    }
    FragColor = texColor;
}