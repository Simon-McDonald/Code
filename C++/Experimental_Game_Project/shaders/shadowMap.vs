#version 330

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec2 iTexCoord;
layout (location = 2) in vec3 iNormal;

uniform mat4 uWVP;
uniform mat4 uLightWMV;
uniform mat4 uWorld;

out vec4 LightSpacePos;                                                              
out vec2 TexCoord0;                                                                  
out vec3 Normal0;                                                                    
out vec3 WorldPos0;  

void main() {
	gl_Position = uWVP * vec4(iPosition, 1.0);
	LightSpacePos = uLightWMV * vec4(iPosition, 1.0);
	TexCoord0 = iTexCoord;
	Normal0 = (uWorld * vec4(iNormal, 1.0)).xyz;
	WorldPos0 = (uWorld * vec4(iPosition, 1.0)).xyz;
}
	