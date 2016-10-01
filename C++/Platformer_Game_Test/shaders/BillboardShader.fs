#version 330

uniform sampler2D gSampler;

in vec2 TexCoord;
out vec4 FragColor;

void main() {
	//FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	FragColor = texture2D(gSampler, TexCoord);
	
	if (FragColor.r == 1 && FragColor.g == 0 && FragColor.b == 1) {
		discard;
	}
}