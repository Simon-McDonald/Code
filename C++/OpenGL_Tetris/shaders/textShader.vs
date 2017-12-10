#version 330 core

layout(location = 0) in int lLetterIndex;
layout(location = 1) in vec2 lLetterOffset;

uniform vec2 uScreenPosition;
uniform vec2 uTextScaling;

out int vLetterIndex;

void main(){
	vLetterIndex = lLetterIndex;
	gl_Position = vec4(uScreenPosition + uTextScaling * lLetterOffset, 0.0, 1.0);
}
