#version 420

uniform sampler2D gActorSkin;

in vec2 gTextureCoord;

out vec4 FragColor;

void main() {
	FragColor = texture2D(gActorSkin, gTextureCoord);	
}
