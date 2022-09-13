#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;

out vec3 vtxpos;
//out vec2 screenCoord;
out vec3 vtxnorm;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * vec4(aPos, 1.0f);
    //screenCoord = (vec2(aPos.x, aPos.y)+ 1.0) / 2.0;
    vtxpos = aPos;
    vtxnorm = aNorm;
}
