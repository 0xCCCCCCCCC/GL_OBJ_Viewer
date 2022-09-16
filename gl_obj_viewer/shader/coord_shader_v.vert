#version 330 core

layout(location = 0) in vec3 cPos;

out float colorType;

uniform mat4 view;
uniform mat4 projection;

void main(){
    colorType = 0;
    if(cPos.y >= 0 && cPos.z == 0)
        colorType = 1;
    else
        if(cPos.y >= 0 && cPos.x == 0)
            colorType = 2;
    //if(cPos.x == 0 && cPos.y == 0 || cPos.x == 0 && cPos.z == 0 || cPos.y == 0 && cPos.z == 0)
    //    colorType = 3;
    gl_Position = projection * view * vec4(cPos * 1.5f, 1.0f);
}

