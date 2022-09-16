#version 330 core

in float colorType;

out vec4 FragColor;

void main(){
    if(colorType == 0)
        FragColor = vec4(.49f, .32f, .12f, 1.0f);
    else{
        if(colorType == 1)
            FragColor = vec4(.4f, .4f, .4f, 1.0f);
        else{
            if(colorType == 2)
                FragColor = vec4(.24f, .24f, .49f, 1.0f);
            else
                FragColor = vec4(.9f, .9f, .9f, 1.0f);
        }
    }
}

