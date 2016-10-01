#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in float texIndex[];

uniform float textureGridDim;

uniform float screenWidth;
uniform float screenHeight;

uniform bool flipVertical;

out vec2 TexCoord;

void main() {
	float polygonWidth = 2 / screenWidth;
	float polygonHeight = 2 / screenHeight;

	vec3 Pos = gl_in[0].gl_Position.xyz;
	Pos.x = Pos.x * polygonWidth;
	Pos.y = Pos.y * polygonHeight;

	float startWidth = texIndex[0] / textureGridDim; //mod(texIndex[0], textureGridDim) / textureGridDim;
	float startHeight = (floor(texIndex[0] / textureGridDim)) / textureGridDim;
	float width = 1.0 / textureGridDim;
	
	Pos.x -= polygonWidth / 2;
	Pos.y -= polygonHeight / 2;
	TexCoord = vec2(flipVertical ? startWidth + width : startWidth, startHeight + width);
	gl_Position = vec4(Pos, 1.0);
	EmitVertex();
	
	Pos.x += polygonWidth;
	TexCoord = vec2(flipVertical ? startWidth : startWidth + width, startHeight + width);
	gl_Position = vec4(Pos, 1.0);
	EmitVertex();

	Pos.x -= polygonWidth;
	Pos.y += polygonHeight;
	TexCoord = vec2(flipVertical ? startWidth + width : startWidth, startHeight);
	gl_Position = vec4(Pos, 1.0);
	EmitVertex();

	Pos.x += polygonWidth;
	TexCoord = vec2(flipVertical ? startWidth : startWidth + width, startHeight);
	gl_Position = vec4(Pos, 1.0);
	EmitVertex();

	EndPrimitive();
}
