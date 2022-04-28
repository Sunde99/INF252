#version 450

in vec2 texturePosition;
uniform float width;
out vec4 fragmentColor;

layout(r32ui, binding = 0) uniform uimage2D histogramImage;

void main(void)
{
//    uint value = imageLoad(histogramImage,ivec2(gl_FragCoord.xy)).r;
//    float scaled = clamp(log(value)/8.0,0.0,1.0);
//    fragmentColor = vec4(scaled,scaled,scaled,1.0);

    uint value = imageLoad(histogramImage,ivec2(gl_FragCoord.x / (width*2.22f))).r;

    float scaled = clamp(log(value)/8.0,0.0,1.0);
    if (scaled<=texturePosition.y){
        fragmentColor = vec4(1);
    }
    else {
        fragmentColor = vec4(0);
    }
}
