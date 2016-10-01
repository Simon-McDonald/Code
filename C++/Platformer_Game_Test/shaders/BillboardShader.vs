#version 330

layout (location = 0) in vec2 Position;
layout (location = 1) in float TextureIndex;

uniform mat4 cameraView;

out float texIndex;

void main() {
	gl_Position = cameraView * vec4(Position, 0.0, 1.0);
	texIndex = TextureIndex;
}
