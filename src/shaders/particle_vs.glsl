#version 460 core

layout(location = 0) in vec2 position;
in mat4 modelViewMatrix;
in vec4 inColor;

out vec4 fsColor;

uniform mat4 projectionMatrix;

void main(void){
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0.0, 1.0);
	fsColor = inColor;
}