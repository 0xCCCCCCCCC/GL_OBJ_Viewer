#version 330 core
#define PI 3.1415926

in vec2 screenCoord;
in vec3 vtxnorm;

//uniform vec2 screenSize;

out vec4 FragColor;

void main(){
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //FragColor = vec4(screenCoord.x, screenCoord.y, 0.2, 1.0);
    FragColor = vec4(vtxnorm.x, vtxnorm.y, vtxnorm.z, 1.0);
}
