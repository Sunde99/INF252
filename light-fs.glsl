#version 430

in vec2 fragCoord;

out vec4 fragColor;

uniform mat4 inverseModelViewMatrix;
uniform vec3 lightPos = vec3(0, 0, 0.0);


void main(void)
{
    gl_FragDepth = 0;
    fragColor = vec4(1, 1, 1, 1);
    vec4 centerLight = vec4(5.5, 11, 0, 0);
    mat4 moveLightToProperPosition = mat4(vec4(0.18, 0, 0, 0), vec4(0, 0.09, 0, 0), vec4(0),vec4(0));
    vec4 newLightPos = vec4(lightPos, 1) * inverseModelViewMatrix;
    vec4 newLightCentered = newLightPos - centerLight;
    vec4 newLight = newLightCentered * moveLightToProperPosition;
//    vec4 newLightPos = () - centerLight) * moveLightToProperPosition;
//    vec4 newLightPos = vec4(lightPos, 1.0) * inverseModelViewMatrix;
//    newLightPos -= vec4(5);
//    newLightPos *= 0.2;
//    newLightPos.y = -newLightPos.y;
    float dist = sqrt(pow(fragCoord.x - newLight.x, 2) + pow(fragCoord.y + newLight.y, 2));

    if (dist < 0.01)//abs(fragCoord.x - lightPos.x) < 0.1 && abs(fragCoord.y - lightPos.y) < 0.1)
    {
        fragColor = vec4(1, 1, 0, 1);
    }
    else {
        gl_FragDepth = 1;
    }
    //        if (abs(fragCoord.x - lightPos.x) < 0.1
//        && abs(fragCoord.y - lightPos.y) < 0.1)
//    {
//        fragColor = vec4(0.5);
//    }
}
