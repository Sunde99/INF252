#version 430

uniform int RAYMARCH_STEPS = 500;
uniform float EPSILON = 0.1;

in vec2 fragCoord;

uniform mat4 MVP;
uniform sampler1D transferFunction;
uniform sampler3D volumeTexture;
uniform vec3 volumeScale;
uniform vec3 volumeSpacing;
uniform vec3 lightDir = vec3(1.0, 0.0, 0.0);
uniform vec4 backgroundColor = vec4(1);
uniform vec3 boxPos;
uniform vec3 boxSize;

out vec4 fragColor;

// https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
vec2 boxIntersection(vec3 ro, vec3 rd, vec3 boxSize, vec3 boxPos){
    ro += boxPos;
    vec3 m = 1.0/rd;
    vec3 n = m*ro;
    vec3 k = abs(m)*boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
    if( tN>tF || tF < 0.0) return vec2(-1.0); // no intersection
    // outNormal = -sign(rdd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
    return vec2( tN, tF );
}

vec4 tf(vec3 p){
    p /= volumeScale * 2.0; // Scale p to [-scale, scale] / 2
    p /= volumeSpacing;
    p += 0.5; // Shift uv's so we go from [-0.5, 0.5] to [0, 1.0]
    float d = texture(volumeTexture, p).r;
    vec4 tex = texture(transferFunction, d);
    return tex;
}

vec3 gradient(vec3 p) {
    return vec3(
        tf(vec3(p.x + EPSILON, p.y, p.z)).a - tf(vec3(p.x - EPSILON, p.y, p.z)).a,
        tf(vec3(p.x, p.y + EPSILON, p.z)).a - tf(vec3(p.x, p.y - EPSILON, p.z)).a,
        tf(vec3(p.x, p.y, p.z + EPSILON)).a - tf(vec3(p.x, p.y, p.z - EPSILON)).a
    );
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}


void main(void)
{
    // Low but non-zero fragDepth to avoid overshadowing the light icon
    gl_FragDepth = 0.001;

    vec4 near = MVP * vec4(fragCoord, -1., 1.);
    near /= near.w;

    vec4 far = MVP * vec4(fragCoord, 1., 1.);
    far /= far.w;

    vec3 rayOrigin = near.xyz;
    vec3 rayDir = normalize(far.xyz - near.xyz);
    fragColor = vec4(.0);

    vec3 maxSize = volumeSpacing*volumeScale;
    vec3 box = maxSize*boxSize - maxSize*boxPos*0.5;
    vec3 box2 = maxSize*boxSize - maxSize*boxPos*0.5;
    vec3 boi = (maxSize - box2) - maxSize*boxPos;
    vec2 boundingBox = boxIntersection(rayOrigin,rayDir,box,boi);

    if (boundingBox.y >= 0.0f && boundingBox.x < boundingBox.y){
        boundingBox.x = max(boundingBox.x,0.0);
        const float stepSize = (boundingBox.y - boundingBox.x)/float(RAYMARCH_STEPS);
        float depth = boundingBox.x;

        for (int i=0; i<RAYMARCH_STEPS; i++){
            vec3 p = rayOrigin + rayDir * depth;
            vec4 tex = tf(p);
            float density = tex.a/10.f;
            vec3 g = gradient(p);
            density = mix(density, density * length(g), 0.5);
            vec3 normal = normalize(g);
            vec3 color = tex.rgb;
            vec3 phong = max(dot(normal, -normalize(lightDir)), 0.15) * color;
            fragColor.rgb += (1.0 - fragColor.a) * phong * density;
            fragColor.a += density;
            if (1.0 < fragColor.a) {
                break;
            }
            depth += stepSize;
        }
    } else{
//        gl_FragDepth = 1;
    }
    if (fragColor.a < 1.0){
//        fragColor += backgroundColor * (1.0-fragColor.a);
         fragColor = vec4(fragColor.rgb * fragColor.a + (1.0 - fragColor.a) * abs(rayDir), 1.0);
    }
}
