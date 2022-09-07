#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;

out vec2 screenCoord;
out vec3 vtxnorm;

void main(){
    gl_Position = vec4(aPos.x, (aPos.y) * 1.78f, -aPos.z, 1.0);
    screenCoord = (vec2(aPos.x, aPos.y)+ 1.0) / 2.0;
    vtxnorm = aNorm;
}
