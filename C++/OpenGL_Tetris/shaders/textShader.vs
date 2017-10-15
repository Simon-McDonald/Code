#version 330 core

layout(location = 0) in int lLetterIndex;
layout(location = 1) in float lLetterOffset;

uniform vec2 uScreenPosition;
uniform float uTextScaling;

out int vLetterIndex;

void main(){
	vLetterIndex = lLetterIndex;
	gl_Position = vec4(uScreenPosition + vec2(uTextScaling * lLetterOffset, 0), 0.0, 1.0);
}
