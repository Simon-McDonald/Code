#version 330

uniform sampler2D gSampler;

in vec2 TexCoord;
out vec4 FragColor;

void main() {
	FragColor = texture2D(gSampler, TexCoord);
	
	if (FragColor.r == 1.0 && FragColor.g == 1.0 && FragColor.b == 1.0) {
		discard;
	}
}