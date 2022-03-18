#version 450

in vec2 texturePosition;
out vec4 fragmentColor;

layout(r32ui, binding = 0) uniform uimage2D histogramImage;

void main(void)
{
    uint value = imageLoad(histogramImage,ivec2(gl_FragCoord.xy)).r;
    float scaled = clamp(log(value)/8.0,0.0,1.0);
    fragmentColor = vec4(scaled,scaled,scaled,1.0);
}
