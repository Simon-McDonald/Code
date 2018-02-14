#version 330

layout (location = 1) in vec2 lBoxCentre;
layout (location = 2) in vec2 lBoxDims;

out vec2 vBoxDims;

void main() {
	gl_Position = vec4(lBoxCentre * 2 - vec2(1.0, 1.0), 0.0, 1.0);
	vBoxDims = lBoxDims * 2;
}
