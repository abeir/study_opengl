#version 450 core

in vec3 vNormal;
in vec3 vWordPos;

out vec4 FragColor;

uniform vec3 boxColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main() {
    // 漫反射
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vWordPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

    // 镜面高光
    vec3 viewDir = normalize(viewPos - vWordPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    // 环境光
    vec3 ambientLight = lightColor * ambientStrength;

    vec3 result = (ambientLight + diffuse + specular) * boxColor;
    FragColor = vec4(result, 1.0);
}