#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in vec2 vBoxDims[];

//uniform float uBorderWidth;
//uniform float uFilterColour;

//out vec3 gFilterColour;

void main() {
	//gFilterColour = vec3(0.2, 0.2, 0.2);
	vec2 blockPosition = gl_in[0].gl_Position.xy - vBoxDims[0] / 2;
	//vec2 blockPosition = vec2(0.0, -0.5);

	gl_Position = vec4(blockPosition, 0.0, 1.0);
	EmitVertex();

	//gl_Position = vec4(blockPosition + vec2(0.5, 0.0), 0.0, 1.0);
	gl_Position = vec4(blockPosition + vec2(vBoxDims[0].x, 0.0), 0.0, 1.0);
	EmitVertex();

	//gl_Position = vec4(blockPosition + vec2(0.0, 0.5), 0.0, 1.0);
	gl_Position = vec4(blockPosition + vec2(0.0, vBoxDims[0].y), 0.0, 1.0);
	EmitVertex();

	//gl_Position = vec4(blockPosition + vec2(0.5, 0.5), 0.0, 1.0);
	gl_Position = vec4(blockPosition + vBoxDims[0], 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}
