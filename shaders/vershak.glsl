#version 330 core

in vec3 position;
in vec3 normal;

out vec3 intensity;


uniform vec3 light_dir;
uniform mat4 uProjModelToWorld;
uniform mat4 uProjWorldToCamera;
uniform mat4 uProjCameraToView;

void main() {
    gl_Position = uProjCameraToView * uProjWorldToCamera * uProjModelToWorld * vec4(position, 1.0);
    float color = max(0.1f, dot(light_dir, normalize(normal)));
    intensity = vec3(color, color, color);
}