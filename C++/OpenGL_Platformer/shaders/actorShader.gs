#version 420

#define M_PI 3.1415926535897932384626433832795

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in vec2 vLimbDimensions[];
in float vLimbRotation[];
in vec2 vLimbOrigin[];

in vec2 vLimbTexOrigin[];
in vec2 vLimbTexDims[];

out vec2 gTextureCoord;

void main() {
	vec2 blockPosition = gl_in[0].gl_Position.xy - vLimbDimensions[0] / 2;
	vec2 blockCentre = gl_in[0].gl_Position.xy;
	vec2 limbDims = vLimbDimensions[0] / 2;
	vec2 limbOffset = vLimbOrigin[0];
	
	vec2 textureOrigin = vLimbTexOrigin[0];
	vec2 textureDims = vLimbTexDims[0];

	float theta = vLimbRotation[0] * M_PI / 180.0;
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	
	mat2 rotMat = mat2(cosTheta, -sinTheta, sinTheta, cosTheta);

	gl_Position = vec4(blockCentre + rotMat * (-limbDims + limbOffset), 0.0, 1.0);
	gTextureCoord = textureOrigin + vec2(0, textureDims.y);
	//gTextureCoord = vec2(0, 1);
	EmitVertex();

	gl_Position = vec4(blockCentre + rotMat * (vec2(limbDims.x, -limbDims.y) + limbOffset), 0.0, 1.0);
	gTextureCoord = textureOrigin + textureDims;
	//gTextureCoord = vec2(1, 1);
	EmitVertex();

	gl_Position = vec4(blockCentre + rotMat * (vec2(-limbDims.x, limbDims.y) + limbOffset), 0.0, 1.0);
	gTextureCoord = textureOrigin; 
	//gTextureCoord = vec2(0, 0);
	EmitVertex();

	gl_Position = vec4(blockCentre + rotMat * (limbDims + limbOffset), 0.0, 1.0);
	gTextureCoord = textureOrigin + vec2(textureDims.x, 0);
	//gTextureCoord = vec2(1, 0);
	EmitVertex();
	
	EndPrimitive();
}
