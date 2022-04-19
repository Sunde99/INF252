#version 450

in vec3 volumePosition;
out vec4 fragmentColor;

uniform sampler3D volumeTexture;

void main(void)
{
    float volumeValue = texture(volumeTexture,vec3(volumePosition.xyz)).r;
//    if (volumeValue<=0.1f){
//        gl_FragDepth = 1.0f;
//    }
//    vec3 light = lightDir;
    fragmentColor = vec4(volumeValue,volumeValue,volumeValue,1.0);
}
