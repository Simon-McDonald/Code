///*
// * Shaders1.h
// *
// *  Created on: Oct 14, 2015
// *      Author: Simon
// */
//
//#ifndef SHADERS1_H_
//#define SHADERS1_H_
//
//#define GLSL(src) "#version 150 core\n" #src
//
/////////////////////////////////
//// Vertex Shader source code //
/////////////////////////////////
//const char* vertexShaderSrc = GLSL(
//	in vec3 position;
//	in vec4 color;
//	in vec2 texcoord;
//
//	out vec4 Color;
//	out vec2 Texcoord;
//
//	uniform mat4 model;
//	uniform mat4 view;
//	uniform mat4 proj;
//	uniform vec3 overrideColor;
//
//	void main() {
//		Color = color * vec4(overrideColor, 1.0f);
//		Texcoord = texcoord;
//		gl_Position = proj * view * model * vec4(position, 1.0);
//	};
//);
//
///////////////////////////////////
//// Fragment Shader source code //
///////////////////////////////////
//const char* fragmentShaderSrc = GLSL(
//	in vec4 Color;
//	in vec2 Texcoord;
//
//	out vec4 outColor;
//
//	uniform sampler2D texKitten;
//	uniform sampler2D texPuppy;
//	uniform float textureTransition;
//
//	void main() {
//		outColor = mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.5);
//		outColor = Color * outColor;
//	};
//);
//
//#endif /* SHADERS1_H_ */
