#version 330

uniform sampler2D gActorSkin;

in vec2 gTextureCoord;

out vec4 FragColor;

void main() {
	//FragColor = vec4(0.5, 0.5, 0.5, 1.0);	
	FragColor = texture2D(gActorSkin, gTextureCoord);	
}
