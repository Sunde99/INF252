#version 450

in vec4 vertexPosition;
out vec2 texturePosition;


void main(void)
{
    gl_Position = vertexPosition;
    texturePosition = 0.5*(vertexPosition.xy+vec2(1.0));
}
