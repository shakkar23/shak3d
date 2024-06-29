#version 330 core

out vec4 color;

varying in vec3 intensity;

void main() {
	color = vec4(intensity, 1.0);
}

