#shader vertexShader
#version 330 core
layout (location = 0) in vec3 inPos;

uniform vec3 uColor;
uniform mat4 model;
out vec3 outColor;

void main(){

    gl_Position = model * vec4(inPos, 1.0f);
    outColor = uColor;
}

#shader fragmentShader
#version 330 core
in vec3 outColor;
out vec4 pixelColor;

void main(){
    pixelColor = vec4(outColor, 1.0f);
}