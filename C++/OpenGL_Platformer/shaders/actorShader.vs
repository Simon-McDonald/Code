#version 420

layout (location = 1) in vec2 lLimbOrigin;
layout (location = 2) in vec2 lLimbDimensions;

struct Node {
	vec2 coord;
	//float x;
	//float y;
	float angle;
	float angle2;
};

layout(std140, binding = 3) uniform NodeList {
	Node nodes[5];
};

out vec2 vLimbDimensions;
out float vLimbRotation;

void main() {
	gl_Position = vec4(lLimbOrigin + nodes[gl_VertexID].coord, 0.0, 1.0);
	vLimbDimensions = lLimbDimensions;
	vLimbRotation = nodes[gl_VertexID].angle;
}
