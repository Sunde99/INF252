#version 430

in vec2 position;

out vec2 fragCoord;
//out mat4 inverseMVP;
//out vec3 lightPos;

//uniform mat4 MVP;
//uniform vec3 lightPos;

void main(void)
{
    fragCoord = position;
    gl_Position = vec4(position,.0,1.);
/*
    inverseMVP = MVP;
    lightPos = lightPos*/;
}
