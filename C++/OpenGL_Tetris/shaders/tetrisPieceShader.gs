#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in vec3 vFilterColour[];
in vec2 vBlockSize[];

in vec2 vTexBaseLoc[];
in vec2 vTexBlockSize[];
in int vNumRotations[];
in int vTexArrayIdx[];
in int vRenderFlag[];

out vec3 gFilterColour;
out vec2 gTextureCoord;
flat out int gTexArrayIdx;

int getIndex(int vertIdx) {
	const int bla[16] = int[](0, 1, 2, 3, 2, 0, 3, 1, 3, 2, 1, 0, 1, 3, 0, 2);

	return bla[vertIdx + vNumRotations[0] * 4];
}

vec2 getBlockOffset(int numSteps) {
	switch(numSteps - numSteps / 4) {
		case 1:
			return vTexBlockSize[0];
		case 2:
			return vec2(0.0, 0.0);			
		case 3:
			return vec2(vTexBlockSize[0].x, 0.0);
		default:
			return vec2(0.0, vTexBlockSize[0].y);
	}
}

void main() {
	if (vRenderFlag[0] == 0) {
		return;
	}

	gTexArrayIdx = vTexArrayIdx[0];
	vec2 blockPosition = gl_in[0].gl_Position.xy;

	gFilterColour = vFilterColour[0];
	
	int rotationsOffset = 0;
	switch(vNumRotations[0]) {
		case 1:
			rotationsOffset = 1;
			break;
		case 2: 
			rotationsOffset = 3;
			break;
		case 3: 
			rotationsOffset = 6;
			break;
	}

	gl_Position = vec4(blockPosition, 0.0, 1.0);
	gTextureCoord = vTexBaseLoc[0] + getBlockOffset(getIndex(0));
	EmitVertex();

	gl_Position = vec4(blockPosition + vec2(vBlockSize[0].x, 0.0), 0.0, 1.0);
	gTextureCoord = vTexBaseLoc[0] + getBlockOffset(getIndex(1));
	EmitVertex();

	gl_Position = vec4(blockPosition + vec2(0.0, vBlockSize[0].y), 0.0, 1.0);
	gTextureCoord = vTexBaseLoc[0] + getBlockOffset(getIndex(2));
	EmitVertex();

	gl_Position = vec4(blockPosition + vBlockSize[0], 0.0, 1.0);
	gTextureCoord = vTexBaseLoc[0] + getBlockOffset(getIndex(3));
	EmitVertex();
	
	EndPrimitive();
}
