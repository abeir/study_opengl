#version 450 core

in vec3 vNormal;
in vec3 vWordPos;
in vec2 vTexCoord;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main() {
    // 漫反射
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.position - vWordPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, vTexCoord)));

    // 镜面高光
    vec3 viewDir = normalize(viewPos - vWordPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, vTexCoord)));

    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoord));

    // 发光贴图
    vec3 emission = texture(material.emission, vTexCoord).rgb;

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}