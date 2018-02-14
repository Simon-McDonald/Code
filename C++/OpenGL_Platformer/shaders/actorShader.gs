#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in vec2 vLimbDimensions[];

out vec2 gTextureCoord;

void main() {
	vec2 blockPosition = gl_in[0].gl_Position.xy - vLimbDimensions[0] / 2;

	gl_Position = vec4(blockPosition, 0.0, 1.0);
	gTextureCoord = vec2(0, 1);
	EmitVertex();

	gl_Position = vec4(blockPosition + vec2(vLimbDimensions[0].x, 0.0), 0.0, 1.0);
	gTextureCoord = vec2(1, 1);
	EmitVertex();

	gl_Position = vec4(blockPosition + vec2(0.0, vLimbDimensions[0].y), 0.0, 1.0);
	gTextureCoord = vec2(0, 0);
	EmitVertex();

	gl_Position = vec4(blockPosition + vLimbDimensions[0], 0.0, 1.0);
	gTextureCoord = vec2(1, 0);
	EmitVertex();
	
	EndPrimitive();
}
