#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void flipColor(){
    FragColor = vec4(vec3(1.0 - texture(screenTexture, vTexCoord)), 1.0);
}

void greyColor(){
    vec4 tmp = texture(screenTexture, vTexCoord);
//    float average = (tmp.r + tmp.g + tmp.b) / 3.0;
    float average = 0.2126 * tmp.r + 0.7152 * tmp.g + 0.0722 * tmp.b;
    FragColor = vec4(average, average, average, 1.0);
}

void kernelColor(){
    vec2 offsets[9] = {
        vec2(-offset, offset), // 左上
        vec2(0.0f, offset), // 正上
        vec2(offset, offset), // 右上
        vec2(-offset, 0.0f), // 左
        vec2(0.0f, 0.0f), // 中
        vec2(offset, 0.0f), // 右
        vec2(-offset, -offset), // 左下
        vec2(0.0f, -offset), // 正下
        vec2(offset, -offset)  // 右下
    };
    float kernel[9] = {
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    };

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, vTexCoord.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++) {
       col += sampleTex[i] * kernel[i];
    }
    FragColor = vec4(col, 1.0);
}

void main() {
//    flipColor();
//    greyColor();
    kernelColor();
}