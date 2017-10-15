#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;

out vec3 inColour;

void main(){
	vec3 plane = vec3(0,1,0);
	float planeValue = -1;

	vec3 rayVector = vertexPosition_modelspace - LightPosition_worldspace;
	
	float scale = (planeValue - dot(plane, vertexPosition_modelspace)) / dot(plane, rayVector);
	
	vec3 groundLocation = vertexPosition_modelspace + rayVector * scale;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(groundLocation,1);
	
	scale = scale / 2;
	
	//if (scale > 0.5) {
	//	scale = 0.5;
	//}
	
	inColour = vec3 (scale, scale, scale);	
}

