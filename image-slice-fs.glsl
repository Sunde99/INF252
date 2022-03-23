#version 430

in vec2 fragCoord;

uniform sampler3D volumeTexture;
uniform mat4 MVP = mat4(1.0f);
uniform vec3 volumeScale = vec3(1., 1., 1.);
uniform vec3 volumeSpacing = vec3(1., 1., 1.);

out vec4 fragColor;

void main(void)
{
    const vec2 uv = fragCoord * 0.5 + 0.5;
        vec4 slice = MVP * vec4(fragCoord, .0, 1.);
        slice /= slice.w;


        // Convert [-1, 1] -> [0, 1]
        vec3 texCoords = slice.xyz * 0.5 + 0.5;
        // Apply volume spacing: [0, 1] -> [-0.5, 0.5] -> volumeSpacing * [-0.5, 0.5] -> 0.5 + volumeSpacing * [-0.5, 0.5]
        texCoords = volumeSpacing * (texCoords - 0.5) / volumeScale + 0.5;
        const float density = texture(volumeTexture, texCoords).r;
        //const vec3 color = texture(transferFunction, density).rgb;

        fragColor = vec4(1,0,0,1);//vec4(density);//vec4(color * density, 1.);
}
