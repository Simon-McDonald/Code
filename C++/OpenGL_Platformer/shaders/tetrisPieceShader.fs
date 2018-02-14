#version 330

uniform sampler2D gSamplerArray[10];

in vec3 gFilterColour;
in vec2 gTextureCoord;
flat in int gTexArrayIdx;

out vec4 FragColor;

void main() {
	vec4 filterColour = vec4((1.0f/255.0f) * gFilterColour, 1.0);
	FragColor = filterColour * texture2D(gSamplerArray[gTexArrayIdx], gTextureCoord);	
}
