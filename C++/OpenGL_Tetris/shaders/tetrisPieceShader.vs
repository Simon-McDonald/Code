#version 330

layout (location = 1) in int lGridPosition;
layout (location = 2) in int lTexturePosition;
layout (location = 3) in int lRotations;
layout (location = 4) in int lTextureIndex;
layout (location = 5) in int lRenderFlag;
layout (location = 6) in ivec3 lFilterColour;

uniform ivec2 uTextureDims[10];

uniform ivec2 uWindowDims;
uniform vec2 uWindowSize;
uniform vec2 uWindowPos;

uniform ivec2 uPieceOffset;
uniform int uBlockMod;

uniform bool uUniformSettings;

uniform ivec3 uFilterColour;
uniform int uNumRotations;
uniform int uTexArrayIdx;


out vec3 vFilterColour;
out vec2 vBlockSize;
out vec2 vTexBaseLoc;
out vec2 vTexBlockSize;
out int vNumRotations;
out int vTexArrayIdx;
out int vRenderFlag;


void main() {

	vec2 blockSize = uWindowSize / uWindowDims;
	vBlockSize = blockSize;
	ivec2 blockIndices = ivec2(mod(lGridPosition, uBlockMod), lGridPosition / uBlockMod);
	ivec2 rotatedIndices;
	
	switch (uNumRotations - 4 * (uNumRotations / 4)) {
		case 1:
			rotatedIndices = ivec2((uBlockMod - 1) - blockIndices.y, blockIndices.x);
			break;
		case 2:
			rotatedIndices = ivec2(uBlockMod - 1, uBlockMod - 1) - blockIndices;
			break;
		case 3:
			rotatedIndices = ivec2(blockIndices.y, (uBlockMod - 1) - blockIndices.x);
			break;
		default:
			rotatedIndices = blockIndices;
			break;
	}
	
	gl_Position = vec4(uWindowPos + (rotatedIndices + uPieceOffset) * blockSize, 0.0, 1.0);
	
	if (uUniformSettings) {
		vRenderFlag = 1;
		vFilterColour = uFilterColour;
		vNumRotations = uNumRotations;
		vTexArrayIdx = uTexArrayIdx;
	} else {
		vRenderFlag = lRenderFlag;
		vFilterColour = lFilterColour;
		vNumRotations = lRotations;
		vTexArrayIdx = lTextureIndex;
	}
	
	ivec2 textureGridSize = uTextureDims[vTexArrayIdx];
	vTexBlockSize = vec2(1.0 / textureGridSize.x, 1.0 / textureGridSize.y);
	vTexBaseLoc = vTexBlockSize * vec2(mod(lTexturePosition, textureGridSize.x), lTexturePosition / textureGridSize.x);
}
