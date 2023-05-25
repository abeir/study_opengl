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

// 定向光
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 点光源
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 衰减
    float constant;
    float linear;
    float quadratic;
};

// 聚光
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 衰减
    float constant;
    float linear;
    float quadratic;
    // 平滑
    float cutOff;
    float outerCutOff;
};

uniform Material material;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

uniform vec3 viewPos;


vec3 clacDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // 漫反射
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vTexCoord));
    // 高光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vTexCoord));
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoord));
    return diffuse + specular + ambient;
}

vec3 clacPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos){
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vTexCoord));
    // 高光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vTexCoord));
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoord));
    // 衰减
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    return diffuse * attenuation + specular * attenuation + ambient * attenuation;
}

vec3 clacSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos){
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vTexCoord));
    // 高光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vTexCoord));
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoord));
    // 平滑边缘
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
    // 衰减
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}


void main() {
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(viewPos - vWorldPos);

    vec3 result = clacDirLight(dirLight, norm, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += clacPointLight(pointLights[i], norm, viewDir, vWorldPos);
    }
    result += clacSpotLight(spotLight, norm, viewDir, vWorldPos);
    FragColor = vec4(result, 1.0);
}