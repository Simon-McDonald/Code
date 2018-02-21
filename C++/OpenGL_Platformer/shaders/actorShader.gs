#version 420

#define M_PI 3.1415926535897932384626433832795

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in vec2 vLimbDimensions[];
in float vLimbRotation[];

out vec2 gTextureCoord;

void main() {
	vec2 blockPosition = gl_in[0].gl_Position.xy - vLimbDimensions[0] / 2;
	vec2 blockCentre = gl_in[0].gl_Position.xy;
	vec2 limbDims = vLimbDimensions[0] / 2;

	float theta = vLimbRotation[0] * M_PI / 180.0;
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	
	mat2 rotMat = mat2(cosTheta, -sinTheta, sinTheta, cosTheta);

	//gl_Position = vec4(blockPosition, 0.0, 1.0);
	//gl_Position = vec4(blockCentre - limbDims, 0.0, 1.0);
	gl_Position = vec4(blockCentre - rotMat * limbDims, 0.0, 1.0);
	gTextureCoord = vec2(0, 1);
	EmitVertex();

	//gl_Position = vec4(blockPosition + vec2(vLimbDimensions[0].x, 0.0), 0.0, 1.0);
	//gl_Position = vec4(blockCentre + vec2(vLimbDimensions[0].x, 0.0), 0.0, 1.0);
	gl_Position = vec4(blockCentre + rotMat * vec2(limbDims.x, -limbDims.y), 0.0, 1.0);
	gTextureCoord = vec2(1, 1);
	EmitVertex();

	//gl_Position = vec4(blockPosition + vec2(0.0, vLimbDimensions[0].y), 0.0, 1.0);
	gl_Position = vec4(blockCentre + rotMat * vec2(-limbDims.x, limbDims.y), 0.0, 1.0);
	gTextureCoord = vec2(0, 0);
	EmitVertex();

	//gl_Position = vec4(blockPosition + vLimbDimensions[0], 0.0, 1.0);
	gl_Position = vec4(blockCentre + rotMat * limbDims, 0.0, 1.0);
	gTextureCoord = vec2(1, 0);
	EmitVertex();
	
	EndPrimitive();
}


void bla2() {
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
