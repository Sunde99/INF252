#version 450
layout(local_size_x=4, local_size_y=4, local_size_z=4) in;

layout(r32f, binding = 0) uniform image3D inputVolumeImage;
layout(r32f, binding = 1) uniform image3D outputVolumeImage;
layout(r32ui, binding = 2) uniform uimage2D outputHistogramImage;

ivec3 offset(ivec3 p, ivec3 o)
{
    return clamp(p+o,ivec3(0,0,0),imageSize(inputVolumeImage)-1);
}

void main(void)
{
    ivec3 coord = ivec3(gl_GlobalInvocationID);

    if (coord.x >= imageSize(inputVolumeImage).x || coord.y >= imageSize(inputVolumeImage).y || coord.z >= imageSize(inputVolumeImage).z)
        return;

    float value = imageLoad(inputVolumeImage,coord).r;

    float left = imageLoad(inputVolumeImage,offset(coord,ivec3(-1,0,0))).r;
    float right = imageLoad(inputVolumeImage,offset(coord,ivec3(1,0,0))).r;
    float bottom = imageLoad(inputVolumeImage,offset(coord,ivec3(0,-1,0))).r;
    float top = imageLoad(inputVolumeImage,offset(coord,ivec3(0,1,0))).r;
    float front = imageLoad(inputVolumeImage,offset(coord,ivec3(0,0,-1))).r;
    float back = imageLoad(inputVolumeImage,offset(coord,ivec3(0,0,1))).r;

    vec3 gradient = vec3(left-right,bottom-top,front-back);
    float magnitude = length(gradient);

    ivec2 pos = ivec2(vec2(value,magnitude)*(imageSize(outputHistogramImage)-vec2(1,1)));
    imageAtomicAdd(outputHistogramImage,pos,1u);

    imageStore(outputVolumeImage,coord,vec4(min(1.0,magnitude),0.0,0.0,0.0));
}
