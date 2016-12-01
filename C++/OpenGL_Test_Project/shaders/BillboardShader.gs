#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 gVP;
uniform vec3 gCameraPos;
out vec2 TexCoord;

void main() {
	vec3 Pos = gl_in[0].gl_Position.xyz;
	//Pos.x = 0;
	//Pos.y = 0;
	//Pos.z = 0;
	//vec3 Pos = vec3(0.0, 0.0, 0.0);
	
	vec3 camVector = gCameraPos - Pos;
	//camVector.y = 0;
	
	vec3 toCamera = normalize(camVector);
	//vec3 toCamera = normalize(-1 * Pos);
	//vec3 toCamera = vec3(0.0, 0.0, -1.0);
	vec3 up = vec3(0.0, 0.0, 1.0);
	vec3 right = cross(toCamera, up);
	right.z = 0;
	//right = normalize(right);
	
	Pos -= 0.5 * right;
	
	
	//Pos.z -= 1.0;
	//gl_Position = gVP * vec4(0.0, 0.0, 0.0, 1.0);//gVP * vec4(Pos, 1.0);
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();
	
	Pos.z += 1.0;

	//gl_Position = gVP * vec4(0.0, 0.0, 1.0, 1.0);//gVP * vec4(Pos, 1.0);
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();
	
	
	Pos += right;
	
	
	Pos.z -= 1.0;
	//gl_Position = gVP * vec4(1.0, 0.0, 0.0, 1.0);//gVP * vec4(Pos, 1.0);
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();
	
	Pos.z += 1.0;
	//gl_Position = gVP * vec4(1.0, 0.0, 1.0, 1.0);//gVP * vec4(Pos, 1.0);
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();
	

	
	EndPrimitive();
}
