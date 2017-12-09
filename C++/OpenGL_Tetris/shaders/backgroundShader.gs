#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 25) out;

in vec2 vBoxDims[];

//uniform float uBorderWidth;
//uniform float uFilterColour;

out vec3 gFilterColour;

const float AR = 540.0 / 600.0;

void generateBoxPrimitive(vec2 centrePosition, vec2 boxDims) {
	vec2 blockBottomRight = centrePosition - boxDims / 2;
	
	gl_Position = vec4(blockBottomRight, 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(blockBottomRight + boxDims * vec2(1.0, 0.0), 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(blockBottomRight + boxDims * vec2(0.0, 1.0), 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(blockBottomRight + boxDims, 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}

void main() {
	vec2 frameCentre = gl_in[0].gl_Position.xy;
	vec2 innerBoxDims = vBoxDims[0];
	vec2 frameWidth = vec2(0.05, 0.05 * AR);

	gFilterColour = vec3(0.2, 0.2, 0.2);
	generateBoxPrimitive(frameCentre, innerBoxDims + frameWidth);

	gFilterColour = vec3(1.0, 1.0, 1.0);
	generateBoxPrimitive(frameCentre, innerBoxDims);
}
