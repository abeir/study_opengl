#version 450 core

in vec3 vWorldPos;
in vec2 vTexCoord;
in vec3 vNormal;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse0;
};

uniform Material material;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {
    vec3 I = normalize(vWorldPos - cameraPos);
    vec3 R = reflect(I, normalize(vNormal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}