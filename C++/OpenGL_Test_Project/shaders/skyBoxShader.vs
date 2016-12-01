#version 330

layout (location = 0) in vec3 lPosition;
                  
uniform mat4 viewMat;
                                                            
out vec3 TexCoord0;                                                                  

void main() {
	gl_Position = vec4(lPosition.xy, 0.999, 1.0);
	vec4 dir = viewMat * vec4(gl_Position.xyz, 1.0);
	

	TexCoord0 = dir.xyz;//vec3 (lPosition.x, lPosition.y, 0.3);
	//TexCoord0 = gl_Position;
}
	