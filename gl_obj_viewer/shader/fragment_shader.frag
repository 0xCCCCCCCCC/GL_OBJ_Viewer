#version 330 core
#define PI 3.1415926

in vec3 vtxpos;
//in vec2 screenCoord;
in vec3 vtxnorm;

//uniform vec2 screenSize;
uniform float ambientStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float shininess;

out vec4 FragColor;

void main(){
    vec3 ambient = vec3(.5f, .5f, .5f) * ambientStrength;
    vec3 diffuse = max(dot((vtxpos - lightPos), normalize(vtxnorm)), .0f) * vec3(1.0f, 1.0f, 1.0f);
    float delta = dot(normalize(viewPos - vtxpos), reflect((vtxpos - lightPos), vtxnorm));
    if(delta > 0){
        vec3 specular = vec3(min(pow(delta, shininess), 1.0f));
        FragColor =  vec4(ambient + min((diffuse + specular), vec3(1, 1, 1)) * lightColor, 1.0f);
    }
    else{
        FragColor = vec4(ambient + diffuse * lightColor, 1.0f);
    }
}
