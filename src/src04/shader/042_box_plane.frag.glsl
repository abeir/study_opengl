#version 450 core

in vec3 vWorldPos;
in vec2 vTexCoord;
in vec3 vNormal;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse0;
};

uniform Material material;


void main() {
    FragColor = texture(material.texture_diffuse0, vTexCoord);
}