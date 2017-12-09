#version 330

in vec3 gFilterColour;

out vec4 FragColor;

void main() {
	FragColor = vec4(gFilterColour, 1.0);
}
