#version 330 core

in vec3 position;
uniform mat4 uProjModelToWorld;
uniform mat4 uProjWorldToCamera;
uniform mat4 uProjCameraToView;

void main() {
    gl_Position = uProjCameraToView * uProjWorldToCamera * uProjModelToWorld * vec4(position, 1.0);

}