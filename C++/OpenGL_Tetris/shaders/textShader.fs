#version 330 core

in vec2 gTextureCoord;

uniform sampler2D uTextSampler;
uniform vec3 uTextColour;

out vec4 fColour;

void main(){
	fColour = texture(uTextSampler, gTextureCoord);	
	if (fColour.r < 0.1 || fColour.g < 0.1 || fColour.b < 0.1) {
		discard;
	} else {
		fColour = vec4(uTextColour, 1.0);
	}
}
