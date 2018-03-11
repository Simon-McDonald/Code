#version 420

layout (location = 1) in vec2 lLimbOrigin;
layout (location = 2) in vec2 lLimbDimensions;
layout (location = 3) in vec2 lLimbTexOrigin;
layout (location = 4) in vec2 lLimbTexDims;

struct Node {
	vec2 coord;
	float angle;
	float angle2;
};

layout(std140, binding = 3) uniform NodeList {
	Node nodes[5];
};

out vec2 vLimbDimensions;
out float vLimbRotation;
out vec2 vLimbOrigin;

out vec2 vLimbTexOrigin;
out vec2 vLimbTexDims;

void main() {
	gl_Position = vec4(nodes[gl_VertexID].coord, 0.0, 1.0);
	vLimbOrigin =lLimbOrigin;
	vLimbDimensions = lLimbDimensions;
	vLimbRotation = nodes[gl_VertexID].angle;
	
	vLimbTexOrigin = lLimbTexOrigin;
	vLimbTexDims = lLimbTexDims;
}
