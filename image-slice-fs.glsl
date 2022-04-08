#version 430

in vec2 fragCoord;

uniform sampler1D transferFunction;
uniform sampler3D volumeTexture;
uniform mat4 MVP;
uniform float depth;
uniform int orientation;
uniform vec3 volumeScale = vec3(1., 1., 1.);
uniform vec3 volumeSpacing = vec3(1., 1., 1.);

out vec4 fragColor;

/*
    Returns the texture coordinates to be used in the volume lookup, based on the depth, orientation, and the fragCoord
*/
vec3 getTexCoord(vec2 fragCoordScaled){
    vec3 texCoords;
    switch(orientation){
    case 0:
        return vec3(fragCoordScaled,depth);
    case 1:
        return vec3(fragCoordScaled.x,depth,fragCoordScaled.y);
    case 2:
        return vec3(depth,fragCoordScaled);
    default:
        return vec3(0);
    }
}

void main(void){
    // Scale the fragCoord so texture is centered and fills the screen
    vec2 fragCoordScaled = (fragCoord * 0.5) + 0.5;

    vec3 texCoord = getTexCoord(fragCoordScaled);

    float depth = texture(volumeTexture,texCoord).r;
    fragColor = vec4(depth);
}
