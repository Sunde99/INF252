#version 430

in vec2 position;

out vec2 fragCoord;

void main(void)
{
    fragCoord = position;
    gl_Position = vec4(position,.0,1.);
}
