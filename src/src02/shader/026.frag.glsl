#version 450 core

in vec3 vWorldPos;
in vec2 vTexCoord;
in vec3 vNormal;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 衰减
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;


void main() {
    float dist = length(light.position - vWorldPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 lightDir = normalize(light.position - vWorldPos);
    vec3 norm = normalize(vNormal);
    // 漫反射
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vTexCoord));

    // 高光
    vec3 viewDir = normalize(viewPos - vWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vTexCoord));

    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoord));

    vec3 result = diffuse * attenuation + specular * attenuation + ambient * attenuation;
    FragColor = vec4(result, 1.0);
}