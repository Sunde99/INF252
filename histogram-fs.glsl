#version 450

in vec2 texturePosition;
out vec4 fragmentColor;

layout(r32ui, binding = 0) uniform uimage2D histogramImage;

void main(void)
{
//    uint value = imageLoad(histogramImage,ivec2(gl_FragCoord.xy)).r;
//    float scaled = clamp(log(value)/8.0,0.0,1.0);
//    fragmentColor = vec4(scaled,scaled,scaled,1.0);

    uint value = imageLoad(histogramImage,ivec2(gl_FragCoord.x)).r;
    float scaled = clamp(log(value)/8.0,0.0,1.0);
    float green = 1;
    if (scaled<=texturePosition.y){
        fragmentColor = vec4(1);

    }
    else {
        fragmentColor = vec4(texturePosition.x,texturePosition.y, 0,1);
    }
}
