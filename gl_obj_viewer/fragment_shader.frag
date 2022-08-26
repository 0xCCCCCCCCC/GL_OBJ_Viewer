#version 330 core
#define PI 3.1415926

in vec2 screenCoord;

//uniform vec2 screenSize;
//uniform sampler2D randomMap;
//uniform sampler2D envMap;

out vec4 FragColor;

struct Ray{
    vec3 ori;
    vec3 dir;
};

//struct Camera

void main(){
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    FragColor = vec4(screenCoord.x, screenCoord.y, 0.2, 1.0);
}
