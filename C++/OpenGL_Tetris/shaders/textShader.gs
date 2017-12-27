#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in int vLetterIndex[];

uniform vec4 letterInformation[128];

uniform vec2 uTextScaling;

out vec2 gTextureCoord;

void main() {
	vec4 letterInfo = letterInformation[vLetterIndex[0]];
	vec2 blockPosition = gl_in[0].gl_Position.xy;

	gl_Position = vec4(blockPosition, 0.0, 1.0);
	gTextureCoord = vec2(letterInfo.x, letterInfo.y) + vec2(0.0, letterInfo.w);
	EmitVertex();

	gl_Position = vec4(blockPosition + vec2(letterInfo.z * uTextScaling.x, 0.0), 0.0, 1.0);
	gTextureCoord = vec2(letterInfo.x, letterInfo.y) + vec2(letterInfo.z, letterInfo.w);
	EmitVertex();

	gl_Position = vec4(blockPosition + vec2(0.0, letterInfo.w * uTextScaling.y), 0.0, 1.0);
	gTextureCoord = vec2(letterInfo.x, letterInfo.y);
	EmitVertex();

	gl_Position = vec4(blockPosition + letterInfo.zw * uTextScaling, 0.0, 1.0);
	gTextureCoord = vec2(letterInfo.x, letterInfo.y) + vec2(letterInfo.z, 0.0);
	EmitVertex();
	
	EndPrimitive();
}
