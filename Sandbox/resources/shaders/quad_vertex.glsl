#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

layout(binding = 0) uniform CameraData {
    mat4 viewProj;
} cameraData;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = cameraData.viewProj * vec4(pos, 1.0);
    fragColor = color;
}