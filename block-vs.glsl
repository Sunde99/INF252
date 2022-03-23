#version 450

uniform mat4 modelViewProjectionMatrix;
uniform vec3 blockPosition;
uniform vec3 blockSize;
uniform vec3 volumeSize;
in vec4 vertexPosition;

out vec3 volumePosition;
out vec3 texturePosition;

void main(void)
{
    vec3 blockCoordinates = blockPosition+(vertexPosition.xyz+vec3(1.0))*blockSize;
    gl_Position = modelViewProjectionMatrix*vec4(blockCoordinates,1.0);

    volumePosition = blockCoordinates;
    
    // here we convert from volume coordinates ([0,0,0]-[width-1,height-1,depth-1]) to texture coordinates ([0,0,0]--[1,1,1])
    texturePosition = (volumePosition.xyz+vec3(0.5))/(volumeSize-vec3(1.0));
}

