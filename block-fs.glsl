#version 450

in vec3 volumePosition; // position in volume coordinates ([0,0,0]-[width-1,height-1,depth-1])
in vec3 texturePosition; // position in texture coordinates ([0,0,0]--[1,1,1])
out vec4 fragmentColor;

uniform sampler3D volumeTexture;

void main(void)
{
    float volumeValue = texture(volumeTexture,vec3(texturePosition.xyz)).r;
    fragmentColor = vec4(texturePosition.xyz*(0.5+volumeValue*0.5),1.0);
}
