#version 330

layout (location = 1) in float TextureIndex;

uniform vec2 position;
uniform mat4 cameraView;

out float texIndex;

void main() {
	gl_Position = cameraView * vec4(position, 0.0, 1.0);
	texIndex = TextureIndex;
}