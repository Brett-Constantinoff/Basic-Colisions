#shader vertexShader
#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 0) in vec3 inCol;
out vec3 outCol;

void main(){
    gl_Position = vec4(inPos, 1.0f);
    outCol = inCol;
}

#shader fragmentShader
#version 330 core
in vec3 outCol;
out vec4 pixelColor;

void main(){
    //pixelColor = vec4(outCol, 1.0f);
    pixelColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}