#version 450 core

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vTexCrood;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    float shininess;
};

uniform Material material;

void main() {
    FragColor = texture(material.texture_diffuse0, vTexCrood);
}