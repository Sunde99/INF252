#version 430

in vec2 fragCoord;

out vec4 fragColor;

uniform mat4 inverseModelViewMatrix;
uniform vec3 lightPos = vec3(0, 0, 0.0);


void main(void)
{
    gl_FragDepth = 0;
    fragColor = vec4(1, 1, 1, 1);
    float dist = sqrt(pow(fragCoord.x - lightPos.x, 2) + pow(fragCoord.y + lightPos.y, 2));

    if (dist < 0.01)
    {
        fragColor = vec4(1, 1, 0, 1);
    }
    else {
        gl_FragDepth = 1;
    }
}
