#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 gVP;
uniform vec3 gCameraPos;
out vec2 TexCoord;

void main() {
	vec3 Pos = gl_in[0].gl_Position.xyz;
	
	//Pos.x -= 0.5;
	//Pos.y -= 0.5;
	Pos = vec3(-0.5, -0.5, 0.0);
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();
	
	//Pos.x += 1.0;
	Pos = vec3(0.5, -0.5, 0.0);
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();

	//Pos.y += 1.0;
	Pos = vec3(0.5, 0.5, 0.0);
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();

	//Pos.x -= 1.0;
	Pos = vec3(-0.5, 0.5, 0.0);
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();

	EndPrimitive();
}
