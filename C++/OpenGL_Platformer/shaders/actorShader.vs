#version 330

layout (location = 1) in vec2 lLimbOrigin;
layout (location = 2) in vec2 lLimbDimensions;

out vec2 vLimbDimensions;

void main() {
	gl_Position = vec4(lLimbOrigin, 0.0, 1.0);
	vLimbDimensions = lLimbDimensions;
}
