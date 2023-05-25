#version 450 core

in vec3 vWorldPos;
in vec2 vTexCoord;
in vec3 vNormal;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
};

uniform Material material;

float near = 0.1;
float far  = 100.0;

void fragBoxTexture(){
    FragColor = texture(material.texture_diffuse0, vTexCoord) * texture(material.texture_specular0, vTexCoord);
}

void fragLinearDepth(float depth){
    float z = depth * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - z * (far - near));
    FragColor = vec4(vec3(linearDepth), 1.0);
}


void main() {
//    fragBoxTexture();
    fragLinearDepth(gl_FragCoord.z);
}